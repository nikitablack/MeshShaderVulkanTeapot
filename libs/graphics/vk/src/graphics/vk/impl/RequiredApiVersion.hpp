#pragma once

#include <cstdint>

namespace graphics::vk::impl {

struct RequiredApiVersion {
    static uint32_t constexpr MAJOR{1};
    static uint32_t constexpr MINOR{3};
};

}  // namespace graphics::vk::impl
