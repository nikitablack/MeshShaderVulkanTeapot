#include <GLFW/glfw3.h>

#include <window/Window.hpp>

namespace window {

auto Window::init(std::string const& title) noexcept -> std::expected<void, std::string> {
    m_width = 2560;
    m_height = 1600;

    if (!glfwInit()) {
        return std::unexpected{"Failed to init GLFW."};
    }

    if (!glfwVulkanSupported()) {
        return std::unexpected{"Vulkan is not supported by GLFW."};
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    m_window = glfwCreateWindow(m_width, m_height, title.c_str(), nullptr, nullptr);
    if (!m_window) {
        return std::unexpected{"Failed to create GLFW window."};
    }

    glfwSetWindowSizeLimits(m_window, 300, 300, GLFW_DONT_CARE, GLFW_DONT_CARE);

    return {};
}

auto Window::destroy() noexcept -> void {
    if (!m_window) {
        return;
    }

    glfwDestroyWindow(m_window);
    glfwTerminate();
}

auto Window::get() const noexcept -> GLFWwindow const* {
    return m_window;
}

auto Window::tick() noexcept -> void {
    glfwPollEvents();
}

auto Window::shouldClose() const noexcept -> bool {
    return glfwWindowShouldClose(m_window);
}

auto Window::minimized() const noexcept -> bool {
    return glfwGetWindowAttrib(m_window, GLFW_ICONIFIED);
}

auto Window::size() noexcept -> SizeInfo {
    int newWidth{};
    int newHeight{};
    glfwGetWindowSize(m_window, &newWidth, &newHeight);

    SizeInfo sizeInfo{};
    sizeInfo.w = static_cast<uint32_t>(newWidth);
    sizeInfo.h = static_cast<uint32_t>(newHeight);
    sizeInfo.resized = (newWidth != m_width) || (newHeight != m_height);

    m_width = newWidth;
    m_height = newHeight;

    return sizeInfo;
}

}  // namespace window