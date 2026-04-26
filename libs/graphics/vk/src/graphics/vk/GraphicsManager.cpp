#include <fmt/core.h>

#include <graphics/vk/GraphicsManager.hpp>
#include <graphics/vk/impl/check_instance_version.hpp>
#include <graphics/vk/impl/check_required_instance_extension.hpp>
#include <graphics/vk/impl/create_device.hpp>
#include <graphics/vk/impl/create_instance.hpp>
#include <graphics/vk/impl/create_surface.hpp>
#include <graphics/vk/impl/create_swapchain.hpp>
#include <graphics/vk/impl/create_swapchain_image_views.hpp>
#include <graphics/vk/impl/features/RequiredFeatures.hpp>
#include <graphics/vk/impl/get_graphics_queue_family.hpp>
#include <graphics/vk/impl/get_present_mode.hpp>
#include <graphics/vk/impl/get_supported_physical_devices.hpp>
#include <graphics/vk/impl/get_surface_capabilities.hpp>
#include <graphics/vk/impl/get_surface_extent.hpp>
#include <graphics/vk/impl/get_surface_format.hpp>
#include <graphics/vk/impl/get_swapchain_images.hpp>
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
    TRY_EXPECTED(uint32_t const graphicsQueueFamily,
                 impl::get_graphics_queue_family(m_physicalDevice, m_surface, REQUIRED_QUEUE_COUNT));

    TRY_EXPECTED(m_device, impl::create_device(m_physicalDevice, graphicsQueueFamily, REQUIRED_QUEUE_COUNT));
    TRY_EXPECTED(m_surfaceFormat, impl::get_surface_format(m_physicalDevice, m_surface));
    TRY_EXPECTED(m_presentMode, impl::get_present_mode(m_physicalDevice, m_surface));

    TRY_EXPECTED_VOID(resize(window));

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

    return {};
}

}  // namespace graphics::vk