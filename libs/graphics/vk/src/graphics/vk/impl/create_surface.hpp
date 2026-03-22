#pragma once

#include <vulkan/vulkan.h>

#include <expected>
#include <string>

namespace window {

class Window;

}

namespace graphics::vk::impl {

[[nodiscard]] auto create_surface(VkInstance instance,  //
                                  window::Window& window  //
                                  ) noexcept -> std::expected<VkSurfaceKHR, std::string>;

}
