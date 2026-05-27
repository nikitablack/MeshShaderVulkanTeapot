#include <fmt/core.h>

#include <graphics/vk/GraphicsManager.hpp>
#include <graphics/vk/impl/allocate_command_buffer.hpp>
#include <graphics/vk/impl/begin_compose_rendering.hpp>
#include <graphics/vk/impl/check_instance_version.hpp>
#include <graphics/vk/impl/check_required_instance_extension.hpp>
#include <graphics/vk/impl/create_command_pool.hpp>
#include <graphics/vk/impl/create_device.hpp>
#include <graphics/vk/impl/create_instance.hpp>
#include <graphics/vk/impl/create_semaphore.hpp>
#include <graphics/vk/impl/create_surface.hpp>
#include <graphics/vk/impl/create_swapchain.hpp>
#include <graphics/vk/impl/create_swapchain_image_views.hpp>
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

    TRY_EXPECTED(m_surfaceFormat, impl::get_surface_format(m_physicalDevice, m_surface));
    TRY_EXPECTED(m_presentMode, impl::get_present_mode(m_physicalDevice, m_surface));

    TRY_EXPECTED_VOID(resize(window));

    TRY_EXPECTED(m_imageAvailableSemaphore, impl::create_semaphore(m_device));
    TRY_EXPECTED(m_renderingFinishedSemaphore, impl::create_semaphore(m_device));
    TRY_EXPECTED(m_commandPool, impl::create_command_pool(m_device, m_queue.familyIndex));

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

    return {};
}

auto GraphicsManager::startFrame(window::Window& window) noexcept -> std::expected<void, std::string> {
    auto const windowSizeInfo{window.size()};

    if (windowSizeInfo.resized) {
        TRY_EXPECTED_VOID(resize(window));
    }

    uint32_t constexpr MAX_REBUILD_COUNT{10};
    uint32_t counter{0};
    impl::GetSwapchainImageIndexResult getImageIndexResult{};
    while (true) {
        TRY_EXPECTED(getImageIndexResult,
                     impl::get_swapchain_image_index(m_device, m_swapchain, m_imageAvailableSemaphore));

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

    return {};
}

auto GraphicsManager::endFrame() noexcept -> std::expected<void, std::string> {
    auto const swapchainImage{m_swapchainImages[m_currSwapchainImageIndex]};
    auto const swapchainImageView{m_swapchainImageViews[m_currSwapchainImageIndex]};

    TRY_EXPECTED(auto const commandBuffer, impl::allocate_command_buffer(m_device, m_commandPool));

    impl::begin_compose_rendering(commandBuffer,  //
                                  swapchainImage,  //
                                  swapchainImageView,  //
                                  m_surfaceExtent);

    vkCmdEndRendering(commandBuffer);

    utils::set_image_barrier(commandBuffer,  //
                             swapchainImage,  //
                             VK_IMAGE_ASPECT_COLOR_BIT,  //
                             VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,  //
                             VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,  //
                             VK_PIPELINE_STAGE_2_NONE,  //
                             VK_ACCESS_2_NONE,  //
                             VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,  //
                             VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        return std::unexpected{"failed to end command buffer"};
    }

    TRY_EXPECTED_VOID(
        impl::submit(commandBuffer, m_queue.queue, m_imageAvailableSemaphore, m_renderingFinishedSemaphore));

    TRY_EXPECTED_VOID(impl::present(m_queue.queue,  //
                                    m_swapchain,  //
                                    m_currSwapchainImageIndex,  //
                                    m_renderingFinishedSemaphore));

    vkQueueWaitIdle(m_queue.queue);

    return {};
}

}  // namespace graphics::vk