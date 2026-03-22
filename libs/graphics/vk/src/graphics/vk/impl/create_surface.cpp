#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <fmt/core.h>

#include <graphics/vk/impl/create_surface.hpp>
#include <window/Window.hpp>

namespace graphics::vk::impl {

[[nodiscard]] auto create_surface(VkInstance instance,  //
                                  window::Window& window  //
                                  ) noexcept -> std::expected<VkSurfaceKHR, std::string> {
    fmt::println("creating surface");

    VkSurfaceKHR surface{VK_NULL_HANDLE};
    if (glfwCreateWindowSurface(instance, window.get(), nullptr, &surface) != VK_SUCCESS) {
        return std::unexpected{"failed to create window surface"};
    }

    return surface;
}

}  // namespace graphics::vk::impl
