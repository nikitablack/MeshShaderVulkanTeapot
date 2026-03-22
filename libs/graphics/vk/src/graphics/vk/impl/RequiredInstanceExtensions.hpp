#pragma once

#include <string>
#include <vector>

namespace graphics::vk::impl {

struct RequiredInstanceExtensions {
    static auto get() noexcept -> std::vector<std::string>;
    static auto print() noexcept -> void;
};

}  // namespace graphics::vk::impl
