#pragma once

#include <expected>
#include <string>

struct GLFWwindow;

namespace window {

class Window {
public:
    struct SizeInfo {
        uint32_t w{0};
        uint32_t h{0};
        bool resized{false};
    };

public:
    Window() = default;
    [[nodiscard]] auto init(std::string const& title) noexcept -> std::expected<void, std::string>;
    auto destroy() noexcept -> void;

public:
    auto get() const noexcept -> GLFWwindow const*;
    auto tick() noexcept -> void;
    auto shouldClose() const noexcept -> bool;
    auto minimized() const noexcept -> bool;
    auto size() noexcept -> SizeInfo;

private:
    GLFWwindow* m_window{nullptr};
    int m_width{0};
    int m_height{0};
};

}  // namespace window
