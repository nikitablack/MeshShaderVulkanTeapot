#include <fmt/core.h>

#include <cstdlib>
#include <graphics/vk/GraphicsManager.hpp>
#include <utils/try_expected.hpp>
#include <window/Window.hpp>

namespace {

auto main_impl() -> std::expected<void, std::string> {
    window::Window window{};
    TRY_EXPECTED_VOID(window.init("Teapot"));

    graphics::vk::GraphicsManager graphicsManager{};
    TRY_EXPECTED_VOID(graphicsManager.init(window));

    while (!window.shouldClose()) {
        window.tick();

        TRY_EXPECTED_VOID(graphicsManager.startFrame(window));
    }

    graphicsManager.destroy();
    window.destroy();

    return {};
}

}  // namespace

auto main() -> int {
    if (auto result{main_impl()}; !result) {
        fmt::println("Fatal error: {}", result.error());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
