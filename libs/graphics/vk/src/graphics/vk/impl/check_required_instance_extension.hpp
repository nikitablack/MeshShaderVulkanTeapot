#pragma once

#include <expected>
#include <string>

namespace graphics::vk::impl {

[[nodiscard]] auto check_required_instance_extension() noexcept -> std::expected<void, std::string>;

}