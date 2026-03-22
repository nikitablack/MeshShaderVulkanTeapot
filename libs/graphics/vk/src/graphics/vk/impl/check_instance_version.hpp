#pragma once

#include <expected>
#include <string>

namespace graphics::vk::impl {

[[nodiscard]] auto check_instance_version() noexcept -> std::expected<void, std::string>;

}