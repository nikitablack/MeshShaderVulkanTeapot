#include <fmt/core.h>

#include <cstdlib>
#include <utils/try_expected.hpp>
#include <window/Window.hpp>

namespace {

auto main_impl() -> std::expected<void, std::string> {
    window::Window window{};
    TRY_EXPECTED_VOID(window.init("Teapot"));

    while (!window.shouldClose()) {
        window.tick();
    }

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
