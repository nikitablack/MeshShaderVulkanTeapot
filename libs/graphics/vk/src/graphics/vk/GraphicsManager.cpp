#include <fmt/core.h>

#include <graphics/vk/GraphicsManager.hpp>
#include <graphics/vk/impl/check_instance_version.hpp>
#include <graphics/vk/impl/check_required_instance_extension.hpp>
#include <graphics/vk/impl/create_instance.hpp>
#include <graphics/vk/impl/create_surface.hpp>
#include <graphics/vk/impl/features/RequiredFeatures.hpp>
#include <graphics/vk/impl/get_supported_physical_devices.hpp>
#include <utils/try_expected.hpp>

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

auto GraphicsManager::changePhysicalDevice(window::Window& /* window */) noexcept -> std::expected<void, std::string> {
    // TODO
    return {};
}

}  // namespace graphics::vk