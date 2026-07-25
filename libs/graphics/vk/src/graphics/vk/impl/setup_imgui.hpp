#pragma once

#include <expected>
#include <string>

namespace window {

class Window;

}

namespace graphics::vk::impl {

[[nodiscard]] auto setup_imgui(window::Window& window) noexcept -> std::expected<void, std::string>;

}
