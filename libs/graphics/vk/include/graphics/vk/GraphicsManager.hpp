#pragma once

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include <expected>
#include <graphics/vk/FrameData.hpp>
#include <graphics/vk/Image2d.hpp>
#include <graphics/vk/VulkanQueue.hpp>
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
    [[nodiscard]] auto endFrame() noexcept -> std::expected<void, std::string>;

private:
    auto destroyDevice() noexcept -> void;

public:
    static uint32_t constexpr FRAMES_IN_FLIGHT{2};
    static uint32_t constexpr DESCRIPTOR_COUNT{100};

private:
    VkInstance m_instance{VK_NULL_HANDLE};
    VkSurfaceKHR m_surface{VK_NULL_HANDLE};
    std::vector<VkPhysicalDevice> m_supportedPhysicalDevices{};

    VkPhysicalDevice m_physicalDevice{VK_NULL_HANDLE};
    VkDevice m_device{VK_NULL_HANDLE};
    VulkanQueue m_queue{};
    VmaAllocator m_allocator{VK_NULL_HANDLE};
    VkSurfaceFormat2KHR m_surfaceFormat{};
    VkPresentModeKHR m_presentMode{};
    VkFormat m_renderTargetFormat{};
    std::vector<VkSemaphore> m_imageAvailableSemaphores{};
    // these semaphores are special:
    // https://vulkan.lunarg.com/doc/view/1.4.313.0/mac/antora/guide/latest/swapchain_semaphore_reuse.html
    std::vector<VkSemaphore> m_renderingFinishedSemaphores{};
    std::vector<VkFence> m_fences{};
    std::vector<VkCommandPool> m_commandPools{};
    std::vector<VkDescriptorPool> m_descriptorPools{};
    VkDescriptorSetLayout m_storageDescriptorSetLayout{VK_NULL_HANDLE};
    VkDescriptorSetLayout m_cisDescriptorSetLayout{VK_NULL_HANDLE};
    VkPipelineLayout m_pipelineLayout{VK_NULL_HANDLE};
    VkSampler m_sampler{VK_NULL_HANDLE};
    VkPipeline m_fullscreenTrianglePipeline{VK_NULL_HANDLE};

    VkSurfaceCapabilities2KHR m_surfaceCapabilities{};
    VkExtent2D m_surfaceExtent{};
    VkSwapchainKHR m_swapchain{VK_NULL_HANDLE};
    std::vector<VkImage> m_swapchainImages{};
    std::vector<VkImageView> m_swapchainImageViews{};
    Image2d m_renderTarget{};

    uint32_t m_frameIndex{0};
    uint32_t m_currSwapchainImageIndex{};
    FrameData m_frameData{};
};

}  // namespace graphics::vk
