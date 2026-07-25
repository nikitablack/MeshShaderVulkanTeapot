#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <fmt/core.h>
#include <imgui.h>

#include <cmath>
#include <cmrc/cmrc.hpp>
#include <graphics/vk/impl/setup_imgui.hpp>
#include <window/Window.hpp>

CMRC_DECLARE(fonts_lib);

namespace graphics::vk::impl {

auto setup_imgui(window::Window& window) noexcept -> std::expected<void, std::string> {
    fmt::println("setting up imgui");

    // visual style
    {
        IMGUI_CHECKVERSION();
        [[maybe_unused]] auto const* const context{ImGui::CreateContext()};

        ImGuiIO& io{ImGui::GetIO()};
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigWindowsMoveFromTitleBarOnly = true;
        io.IniFilename = nullptr;
        io.LogFilename = nullptr;
        io.BackendFlags |= ImGuiBackendFlags_RendererHasTextures;

        auto& imguiColors{ImGui::GetStyle().Colors};

        // Window
        imguiColors[ImGuiCol_WindowBg] = ImVec4{0.1f, 0.105f, 0.11f, 1.0f};

        // Headers
        imguiColors[ImGuiCol_Header] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
        imguiColors[ImGuiCol_HeaderHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
        imguiColors[ImGuiCol_HeaderActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

        // Buttons
        imguiColors[ImGuiCol_Button] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
        imguiColors[ImGuiCol_ButtonHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
        imguiColors[ImGuiCol_ButtonActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

        // Frame BG
        imguiColors[ImGuiCol_FrameBg] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
        imguiColors[ImGuiCol_FrameBgHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
        imguiColors[ImGuiCol_FrameBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

        // Tabs
        imguiColors[ImGuiCol_Tab] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
        imguiColors[ImGuiCol_TabHovered] = ImVec4{0.38f, 0.3805f, 0.381f, 1.0f};
        imguiColors[ImGuiCol_TabSelected] = ImVec4{0.28f, 0.2805f, 0.281f, 1.0f};
        imguiColors[ImGuiCol_TabDimmed] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
        imguiColors[ImGuiCol_TabDimmedSelected] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};

        // Title
        imguiColors[ImGuiCol_TitleBg] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
        imguiColors[ImGuiCol_TitleBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
        imguiColors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    }

    // load font
    {
        int windowWidth{};
        int windowHeight{};
        glfwGetWindowSize(window.get(), &windowWidth, &windowHeight);

        int fbWidth{};
        int fbHeight{};
        glfwGetFramebufferSize(window.get(), &fbWidth, &fbHeight);

        float const logicalToPhysicalPixelRatio{static_cast<float>(windowWidth) / static_cast<float>(windowHeight)};

        float constexpr BASE_IMGUI_FONT_SIZE{32.0f};
        auto const imguiFontSize{std::ceil(BASE_IMGUI_FONT_SIZE * logicalToPhysicalPixelRatio)};

        auto const fs{cmrc::fonts_lib::get_filesystem()};

        ImVector<ImWchar> openSansRanges{};

        if (!fs.exists("OpenSans-Regular.ttf")) {
            return std::unexpected{"failed to load font"};
        }

        auto openSansFont{fs.open("OpenSans-Regular.ttf")};

        ImGuiIO& io{ImGui::GetIO()};

        ImFontGlyphRangesBuilder builder{};
        builder.AddRanges(io.Fonts->GetGlyphRangesDefault());
        builder.BuildRanges(&openSansRanges);

        ImFontConfig fontConfig{};
        fontConfig.FontDataOwnedByAtlas = true;

        char* fontData{static_cast<char*>(ImGui::MemAlloc(openSansFont.size()))};
        std::memcpy(fontData, openSansFont.begin(), openSansFont.size());

        io.FontDefault = io.Fonts->AddFontFromMemoryTTF(fontData,  //
                                                        static_cast<int>(openSansFont.size()),  //
                                                        imguiFontSize,  //
                                                        &fontConfig,  //
                                                        openSansRanges.Data);
    }

    ImGui_ImplGlfw_InitForVulkan(window.get(), true);

    return {};
}

}  // namespace graphics::vk::impl
