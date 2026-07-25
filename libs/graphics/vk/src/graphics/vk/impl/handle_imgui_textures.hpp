#pragma once

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include <expected>
#include <graphics/vk/Image2d.hpp>
#include <string>
#include <vector>

namespace graphics::vk::impl {

[[nodiscard]] auto handle_imgui_textures(VkDevice device,  //
                                         VmaAllocator allocator,  //
                                         VkCommandPool commandPool,  //
                                         VkQueue queue,  //
                                         std::vector<Image2d>& imguiImagesInOut  //
                                         ) noexcept -> std::expected<void, std::string>;

}
