#include <fmt/core.h>
#include <imgui.h>

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

        // ImGui::SetNextWindowPos(ImVec2{0.0f, 0.0f});
        // ImGui::Begin("test");
        // ImGui::Text("Hello, World!");
        // if (ImGui::Button("Click Me")) {
        //     fmt::println("button was clicked");
        // }
        // ImGui::End();

        ImGui::ShowDemoWindow();

        TRY_EXPECTED_VOID(graphicsManager.endFrame());
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
