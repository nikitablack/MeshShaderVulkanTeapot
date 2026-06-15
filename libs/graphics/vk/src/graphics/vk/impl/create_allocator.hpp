#pragma once

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include <expected>
#include <string>

namespace graphics::vk::impl {

[[nodiscard]] auto create_allocator(VkInstance instance,  //
                                    VkPhysicalDevice physicalDevice,  //
                                    VkDevice device  //
                                    ) noexcept -> std::expected<VmaAllocator, std::string>;

}
