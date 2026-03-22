#pragma once

#include <vulkan/vulkan.h>

#include <expected>
#include <string>
#include <vector>

namespace window {

class Window;

}

namespace graphics::vk {

class GraphicsManager {
public:
    GraphicsManager() = default;

public:
    [[nodiscard]] auto init(window::Window& window) noexcept -> std::expected<void, std::string>;

    auto changePhysicalDevice(window::Window& window) noexcept -> std::expected<void, std::string>;

private:
    VkInstance m_instance{VK_NULL_HANDLE};
    VkSurfaceKHR m_surface{VK_NULL_HANDLE};
    std::vector<VkPhysicalDevice> m_supportedPhysicalDevices{};
};

}  // namespace graphics::vk
