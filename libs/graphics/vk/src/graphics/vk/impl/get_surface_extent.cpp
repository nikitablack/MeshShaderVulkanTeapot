#include <GLFW/glfw3.h>

#include <algorithm>
#include <graphics/vk/impl/get_surface_extent.hpp>
#include <limits>
#include <window/Window.hpp>

namespace graphics::vk::impl {

auto get_surface_extent(VkSurfaceCapabilities2KHR const& surfaceCapabilities,  //
                        window::Window& window) noexcept -> VkExtent2D {
    int windowWidth{};
    int windowHeight{};
    glfwGetFramebufferSize(window.get(), &windowWidth, &windowHeight);

    VkExtent2D extent{};

    if (surfaceCapabilities.surfaceCapabilities.currentExtent.width == 0xFFFFFFFF) {
        extent.width = std::max(
            surfaceCapabilities.surfaceCapabilities.minImageExtent.width,
            std::min(surfaceCapabilities.surfaceCapabilities.maxImageExtent.width, static_cast<uint32_t>(windowWidth)));

        extent.height = std::max(surfaceCapabilities.surfaceCapabilities.minImageExtent.height,
                                 std::min(surfaceCapabilities.surfaceCapabilities.maxImageExtent.height,
                                          static_cast<uint32_t>(windowHeight)));
    } else {
        extent = surfaceCapabilities.surfaceCapabilities.currentExtent;
    }

    return extent;
}

}  // namespace graphics::vk::impl
