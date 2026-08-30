#include <GLFW/glfw3.h>
#include <fmt/core.h>
#include <vulkan/vulkan.h>

#include <graphics/vk/impl/RequiredDeviceExtensions.hpp>

namespace graphics::vk::impl {

auto RequiredDeviceExtensions::get() noexcept -> std::vector<std::string> {
    std::vector<std::string> extensions{};

    extensions.push_back(VK_EXT_MESH_SHADER_EXTENSION_NAME);
    extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    extensions.push_back(VK_KHR_MAINTENANCE_5_EXTENSION_NAME);

    return extensions;
}

auto RequiredDeviceExtensions::print() noexcept -> void {
    fmt::println("required device extensions:");

    for (auto const& ext : get()) {
        fmt::println("\t{}", ext);
    }
}

}  // namespace graphics::vk::impl
