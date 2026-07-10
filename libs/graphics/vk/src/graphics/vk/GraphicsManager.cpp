#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>
//

#include <fmt/core.h>

#include <graphics/vk/GraphicsManager.hpp>
#include <graphics/vk/impl/allocate_command_buffer.hpp>
#include <graphics/vk/impl/allocate_descriptor_set.hpp>
#include <graphics/vk/impl/begin_compose_rendering.hpp>
#include <graphics/vk/impl/begin_render_target_rendering.hpp>
#include <graphics/vk/impl/check_instance_version.hpp>
#include <graphics/vk/impl/check_required_instance_extension.hpp>
#include <graphics/vk/impl/create_allocator.hpp>
#include <graphics/vk/impl/create_command_pool.hpp>
#include <graphics/vk/impl/create_descriptor_pool.hpp>
#include <graphics/vk/impl/create_descriptor_set_layout.hpp>
#include <graphics/vk/impl/create_device.hpp>
#include <graphics/vk/impl/create_fence.hpp>
#include <graphics/vk/impl/create_fullscreen_triangle_pipeline.hpp>
#include <graphics/vk/impl/create_instance.hpp>
#include <graphics/vk/impl/create_pipeline_layout.hpp>
#include <graphics/vk/impl/create_sampler.hpp>
#include <graphics/vk/impl/create_semaphore.hpp>
#include <graphics/vk/impl/create_surface.hpp>
#include <graphics/vk/impl/create_swapchain.hpp>
#include <graphics/vk/impl/create_swapchain_image_views.hpp>
#include <graphics/vk/impl/draw_fullscreen_triangle.hpp>
#include <graphics/vk/impl/features/RequiredFeatures.hpp>
#include <graphics/vk/impl/get_graphics_queue_family.hpp>
#include <graphics/vk/impl/get_present_mode.hpp>
#include <graphics/vk/impl/get_queue.hpp>
#include <graphics/vk/impl/get_supported_physical_devices.hpp>
#include <graphics/vk/impl/get_surface_capabilities.hpp>
#include <graphics/vk/impl/get_surface_extent.hpp>
#include <graphics/vk/impl/get_surface_format.hpp>
#include <graphics/vk/impl/get_swapchain_image_index.hpp>
#include <graphics/vk/impl/get_swapchain_images.hpp>
#include <graphics/vk/impl/present.hpp>
#include <graphics/vk/impl/submit.hpp>
#include <graphics/vk/utils/barrier_helper.hpp>
#include <utils/try_expected.hpp>
#include <window/Window.hpp>

namespace graphics::vk {

auto GraphicsManager::init(window::Window& window) noexcept -> std::expected<void, std::string> {
    fmt::println("initializing Vulkan graphics manager");

    TRY_EXPECTED_VOID(impl::check_instance_version());
    TRY_EXPECTED_VOID(impl::check_required_instance_extension());
    TRY_EXPECTED(m_instance, impl::create_instance());
    TRY_EXPECTED(m_surface, impl::create_surface(m_instance, window));
    TRY_EXPECTED(m_supportedPhysicalDevices, impl::get_supported_physical_devices(m_instance));
    TRY_EXPECTED_VOID(changePhysicalDevice(window));

    return {};
}

auto GraphicsManager::destroy() noexcept -> void {
    if (!m_instance) {
        return;
    }

    fmt::println("destroying");

    // TODO: take care of synchronization

    destroyDevice();

    vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
    m_surface = VK_NULL_HANDLE;

    vkDestroyInstance(m_instance, nullptr);
    m_instance = VK_NULL_HANDLE;
}

auto GraphicsManager::destroyDevice() noexcept -> void {
    if (!m_device) {
        return;
    }

    vkDestroyPipeline(m_device, m_fullscreenTrianglePipeline, nullptr);
    m_fullscreenTrianglePipeline = VK_NULL_HANDLE;

    vkDestroySampler(m_device, m_sampler, nullptr);
    m_sampler = VK_NULL_HANDLE;

    vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);
    m_pipelineLayout = VK_NULL_HANDLE;

    for (auto const descriptorPool : m_descriptorPools) {
        vkDestroyDescriptorPool(m_device, descriptorPool, nullptr);
    }
    m_descriptorPools.clear();

    vkDestroyDescriptorSetLayout(m_device, m_storageDescriptorSetLayout, nullptr);
    m_storageDescriptorSetLayout = VK_NULL_HANDLE;

    vkDestroyDescriptorSetLayout(m_device, m_cisDescriptorSetLayout, nullptr);
    m_cisDescriptorSetLayout = VK_NULL_HANDLE;

    m_renderTarget.destroy();

    for (auto const semaphore : m_imageAvailableSemaphores) {
        vkDestroySemaphore(m_device, semaphore, nullptr);
    }
    m_imageAvailableSemaphores.clear();

    for (auto const semaphore : m_renderingFinishedSemaphores) {
        vkDestroySemaphore(m_device, semaphore, nullptr);
    }
    m_renderingFinishedSemaphores.clear();

    for (auto const fence : m_fences) {
        vkDestroyFence(m_device, fence, nullptr);
    }
    m_fences.clear();

    for (auto const commandPool : m_commandPools) {
        vkDestroyCommandPool(m_device, commandPool, nullptr);
    }
    m_commandPools.clear();

    for (auto const view : m_swapchainImageViews) {
        vkDestroyImageView(m_device, view, nullptr);
    }
    m_swapchainImageViews.clear();

    vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
    m_swapchain = VK_NULL_HANDLE;

    vkDestroyDevice(m_device, nullptr);
    m_device = VK_NULL_HANDLE;
}

auto GraphicsManager::changePhysicalDevice(window::Window& window) noexcept -> std::expected<void, std::string> {
    fmt::println("changing physical device");

    destroyDevice();

    m_physicalDevice = m_supportedPhysicalDevices[0];

    uint32_t constexpr REQUIRED_QUEUE_COUNT{1};
    uint32_t constexpr QUEUE_INDEX{0};
    static_assert(QUEUE_INDEX < REQUIRED_QUEUE_COUNT);

    TRY_EXPECTED(uint32_t const graphicsQueueFamily,
                 impl::get_graphics_queue_family(m_physicalDevice, m_surface, REQUIRED_QUEUE_COUNT));

    TRY_EXPECTED(m_device, impl::create_device(m_physicalDevice, graphicsQueueFamily, REQUIRED_QUEUE_COUNT));

    m_queue.familyIndex = graphicsQueueFamily;
    m_queue.queue = impl::get_queue(m_device, graphicsQueueFamily, QUEUE_INDEX);

    TRY_EXPECTED(m_allocator, impl::create_allocator(m_instance, m_physicalDevice, m_device));
    TRY_EXPECTED(m_surfaceFormat, impl::get_surface_format(m_physicalDevice, m_surface));
    TRY_EXPECTED(m_presentMode, impl::get_present_mode(m_physicalDevice, m_surface));

    m_renderTargetFormat = VK_FORMAT_R8G8B8A8_SRGB;

    TRY_EXPECTED_VOID(resize(window));

    m_imageAvailableSemaphores.clear();
    m_imageAvailableSemaphores.reserve(FRAMES_IN_FLIGHT);
    for (uint32_t i{0}; i < FRAMES_IN_FLIGHT; ++i) {
        TRY_EXPECTED(auto const semaphore, impl::create_semaphore(m_device));
        m_imageAvailableSemaphores.push_back(semaphore);
    }

    // special case
    m_renderingFinishedSemaphores.clear();
    m_renderingFinishedSemaphores.reserve(m_swapchainImages.size());
    for (uint32_t i{0}; i < m_swapchainImages.size(); ++i) {
        TRY_EXPECTED(auto const semaphore, impl::create_semaphore(m_device));
        m_renderingFinishedSemaphores.push_back(semaphore);
    }

    m_fences.clear();
    m_fences.reserve(FRAMES_IN_FLIGHT);
    for (uint32_t i{0}; i < FRAMES_IN_FLIGHT; ++i) {
        TRY_EXPECTED(auto const fence, impl::create_fence(m_device));
        m_fences.push_back(fence);
    }

    m_commandPools.clear();
    m_commandPools.reserve(FRAMES_IN_FLIGHT);
    for (uint32_t i{0}; i < FRAMES_IN_FLIGHT; ++i) {
        TRY_EXPECTED(auto const commandPool, impl::create_command_pool(m_device, m_queue.familyIndex));
        m_commandPools.push_back(commandPool);
    }

    m_descriptorPools.clear();
    m_descriptorPools.reserve(FRAMES_IN_FLIGHT);
    for (uint32_t i{0}; i < FRAMES_IN_FLIGHT; ++i) {
        TRY_EXPECTED(auto const descriptorPool, impl::create_descriptor_pool(m_device, DESCRIPTOR_COUNT));
        m_descriptorPools.push_back(descriptorPool);
    }

    TRY_EXPECTED(m_storageDescriptorSetLayout,
                 impl::create_descriptor_set_layout(m_device,  //
                                                    VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,  //
                                                    DESCRIPTOR_COUNT));

    TRY_EXPECTED(m_cisDescriptorSetLayout,
                 impl::create_descriptor_set_layout(m_device,  //
                                                    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,  //
                                                    DESCRIPTOR_COUNT));

    TRY_EXPECTED(m_pipelineLayout, impl::create_pipeline_layout(m_device,  //
                                                                m_storageDescriptorSetLayout,  //
                                                                m_cisDescriptorSetLayout));

    TRY_EXPECTED(m_sampler, impl::create_sampler(m_device));

    TRY_EXPECTED(m_fullscreenTrianglePipeline, impl::create_fullscreen_triangle_pipeline(
                                                   m_device, m_pipelineLayout, m_surfaceFormat.surfaceFormat.format));

    return {};
}

auto GraphicsManager::resize(window::Window& window) noexcept -> std::expected<void, std::string> {
    fmt::println("resizing");

    TRY_EXPECTED(m_surfaceCapabilities, impl::get_surface_capabilities(m_physicalDevice, m_surface));
    m_surfaceExtent = impl::get_surface_extent(m_surfaceCapabilities, window);

    for (auto const view : m_swapchainImageViews) {
        vkDestroyImageView(m_device, view, nullptr);
    }
    m_swapchainImageViews.clear();

    TRY_EXPECTED(m_swapchain, impl::create_swapchain(m_device, m_surface, m_surfaceFormat, m_presentMode,
                                                     m_surfaceCapabilities, m_surfaceExtent, m_swapchain));

    TRY_EXPECTED(m_swapchainImages, impl::get_swapchain_images(m_device, m_swapchain));

    TRY_EXPECTED(m_swapchainImageViews,
                 impl::create_swaphain_image_views(m_device, m_swapchainImages, m_surfaceFormat.surfaceFormat.format));

    m_renderTarget.destroy();

    TRY_EXPECTED_VOID(m_renderTarget.init(m_allocator,  //
                                          m_device,  //
                                          m_renderTargetFormat,  //
                                          VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,  //
                                          VK_IMAGE_ASPECT_COLOR_BIT,  //
                                          m_surfaceExtent.width,  //
                                          m_surfaceExtent.height));

    return {};
}

auto GraphicsManager::startFrame(window::Window& window) noexcept -> std::expected<void, std::string> {
    auto const imageAvailableSemaphore{m_imageAvailableSemaphores[m_frameIndex]};
    auto const fence{m_fences[m_frameIndex]};

    if (vkWaitForFences(m_device, 1, &fence, VK_TRUE, std::numeric_limits<uint64_t>::max()) != VK_SUCCESS) {
        return std::unexpected{fmt::format("failed to wait for the fence {}", m_frameIndex)};
    }

    if (vkResetFences(m_device, 1, &fence) != VK_SUCCESS) {
        return std::unexpected{fmt::format("failed to reset the fence {}", m_frameIndex)};
    }

    auto const windowSizeInfo{window.size()};

    if (windowSizeInfo.resized) {
        TRY_EXPECTED_VOID(resize(window));
    }

    uint32_t constexpr MAX_REBUILD_COUNT{10};
    uint32_t counter{0};
    impl::GetSwapchainImageIndexResult getImageIndexResult{};
    while (true) {
        TRY_EXPECTED(getImageIndexResult,
                     impl::get_swapchain_image_index(m_device, m_swapchain, imageAvailableSemaphore));

        if (getImageIndexResult.rebuildSwapchain) {
            TRY_EXPECTED_VOID(resize(window));
        } else {
            break;
        }

        ++counter;

        if (counter >= MAX_REBUILD_COUNT) {
            return std::unexpected{"failed to resize"};
        }
    }

    m_currSwapchainImageIndex = getImageIndexResult.index;

    auto const commandPool{m_commandPools[m_frameIndex]};
    vkResetCommandPool(m_device, commandPool, 0);

    TRY_EXPECTED(m_frameData.commandBuffer, impl::allocate_command_buffer(m_device, commandPool));

    auto const descriptorPool{m_descriptorPools[m_frameIndex]};
    vkResetDescriptorPool(m_device, descriptorPool, 0);

    TRY_EXPECTED(m_frameData.storageDescriptorSet,
                 impl::allocate_descriptor_set(m_device,  //
                                               descriptorPool,  //
                                               m_storageDescriptorSetLayout,  //
                                               DESCRIPTOR_COUNT));

    TRY_EXPECTED(m_frameData.cisDescriptorSet,
                 impl::allocate_descriptor_set(m_device,  //
                                               descriptorPool,  //
                                               m_cisDescriptorSetLayout,  //
                                               DESCRIPTOR_COUNT));

    m_frameData.cisDescriptorCounter = 0;
    m_frameData.storageDescriptorCounter = 0;

    // bind storage descriptor set
    vkCmdBindDescriptorSets(m_frameData.commandBuffer,  //
                            VK_PIPELINE_BIND_POINT_GRAPHICS,  //
                            m_pipelineLayout,  //
                            0,  //
                            1,  //
                            &m_frameData.storageDescriptorSet,  //
                            0,  //
                            nullptr);

    // bind cis descriptor set
    vkCmdBindDescriptorSets(m_frameData.commandBuffer,  //
                            VK_PIPELINE_BIND_POINT_GRAPHICS,  //
                            m_pipelineLayout,  //
                            1,  //
                            1,  //
                            &m_frameData.cisDescriptorSet,  //
                            0,  //
                            nullptr);

    impl::begin_render_target_rendering(m_frameData.commandBuffer,  //
                                        m_renderTarget,  //
                                        {0.2f, 0.8f, 0.2f, 1.0f},  //
                                        m_surfaceExtent);

    return {};
}

auto GraphicsManager::endFrame() noexcept -> std::expected<void, std::string> {
    auto const swapchainImage{m_swapchainImages[m_currSwapchainImageIndex]};
    auto const swapchainImageView{m_swapchainImageViews[m_currSwapchainImageIndex]};
    auto const renderingFinishedSemaphore{m_renderingFinishedSemaphores[m_currSwapchainImageIndex]};

    auto const imageAvailableSemaphore{m_imageAvailableSemaphores[m_frameIndex]};
    auto const fence{m_fences[m_frameIndex]};

    // end render target rendering
    vkCmdEndRendering(m_frameData.commandBuffer);

    impl::begin_compose_rendering(m_frameData.commandBuffer,  //
                                  m_renderTarget,  //
                                  swapchainImage,  //
                                  swapchainImageView,  //
                                  m_surfaceExtent);

    impl::draw_fullscreen_triangle(m_device,  //
                                   m_fullscreenTrianglePipeline,  //
                                   m_pipelineLayout,  //
                                   m_sampler,  //
                                   m_renderTarget,  //
                                   m_surfaceExtent,  //
                                   m_frameData);

    vkCmdEndRendering(m_frameData.commandBuffer);

    utils::set_image_barrier(m_frameData.commandBuffer,  //
                             swapchainImage,  //
                             VK_IMAGE_ASPECT_COLOR_BIT,  //
                             VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,  //
                             VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,  //
                             VK_PIPELINE_STAGE_2_NONE,  //
                             VK_ACCESS_2_NONE,  //
                             VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,  //
                             VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

    if (vkEndCommandBuffer(m_frameData.commandBuffer) != VK_SUCCESS) {
        return std::unexpected{"failed to end command buffer"};
    }

    TRY_EXPECTED_VOID(impl::submit(m_frameData.commandBuffer,  //
                                   m_queue.queue,  //
                                   imageAvailableSemaphore,  //
                                   renderingFinishedSemaphore,  //
                                   fence));

    TRY_EXPECTED_VOID(impl::present(m_queue.queue,  //
                                    m_swapchain,  //
                                    m_currSwapchainImageIndex,  //
                                    renderingFinishedSemaphore));

    vkQueueWaitIdle(m_queue.queue);

    m_frameIndex = (m_frameIndex + 1) % FRAMES_IN_FLIGHT;

    return {};
}

}  // namespace graphics::vk