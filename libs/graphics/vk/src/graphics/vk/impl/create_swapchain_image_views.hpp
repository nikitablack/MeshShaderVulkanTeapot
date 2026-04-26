#pragma once

#include <vulkan/vulkan.h>

#include <expected>
#include <string>
#include <vector>

namespace graphics::vk::impl {

[[nodiscard]] auto create_swaphain_image_views(VkDevice device,  //
                                               std::vector<VkImage> const& images,  //
                                               VkFormat surfaceFormat  //
                                               ) noexcept -> std::expected<std::vector<VkImageView>, std::string>;

}
