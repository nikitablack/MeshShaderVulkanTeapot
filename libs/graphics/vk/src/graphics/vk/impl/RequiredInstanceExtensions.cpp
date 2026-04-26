#include <GLFW/glfw3.h>
#include <fmt/core.h>
#include <vulkan/vulkan.h>

#include <graphics/vk/impl/RequiredInstanceExtensions.hpp>

namespace graphics::vk::impl {

auto RequiredInstanceExtensions::get() noexcept -> std::vector<std::string> {
    std::vector<std::string> result{};

    uint32_t glfwExtensionCount{};
    char const* const* const glfwExtensions{glfwGetRequiredInstanceExtensions(&glfwExtensionCount)};

    result.reserve(glfwExtensionCount);

    for (uint32_t i{0}; i < glfwExtensionCount; ++i) {
        result.push_back(glfwExtensions[i]);
    }

    result.push_back(VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME);

    return result;
}

auto RequiredInstanceExtensions::print() noexcept -> void {
    fmt::println("required instance extensions:");

    for (auto const& ext : get()) {
        fmt::println("\t{}", ext);
    }
}

}  // namespace graphics::vk::impl
