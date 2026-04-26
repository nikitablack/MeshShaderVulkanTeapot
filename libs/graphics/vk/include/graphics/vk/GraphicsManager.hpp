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
    auto destroy() noexcept -> void;

    [[nodiscard]] auto changePhysicalDevice(window::Window& window) noexcept -> std::expected<void, std::string>;
    [[nodiscard]] auto resize(window::Window& window) noexcept -> std::expected<void, std::string>;
    [[nodiscard]] auto startFrame(window::Window& window) noexcept -> std::expected<void, std::string>;

private:
    auto destroyDevice() noexcept -> void;

private:
    VkInstance m_instance{VK_NULL_HANDLE};
    VkSurfaceKHR m_surface{VK_NULL_HANDLE};
    std::vector<VkPhysicalDevice> m_supportedPhysicalDevices{};

    VkPhysicalDevice m_physicalDevice{VK_NULL_HANDLE};
    VkDevice m_device{VK_NULL_HANDLE};
    VkSurfaceFormat2KHR m_surfaceFormat{};
    VkPresentModeKHR m_presentMode{};

    VkSurfaceCapabilities2KHR m_surfaceCapabilities{};
    VkExtent2D m_surfaceExtent{};
    VkSwapchainKHR m_swapchain{VK_NULL_HANDLE};
    std::vector<VkImage> m_swapchainImages{};
    std::vector<VkImageView> m_swapchainImageViews{};
};

}  // namespace graphics::vk
