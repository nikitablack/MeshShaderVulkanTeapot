#include <fmt/core.h>

#include <graphics/vk/impl/RequiredInstanceExtensions.hpp>
#include <graphics/vk/impl/create_instance.hpp>
#include <vulkan/utility/vk_struct_helper.hpp>

namespace graphics::vk::impl {

auto create_instance() noexcept -> std::expected<VkInstance, std::string> {
    fmt::println("creating instance");

    const auto& requiredExtensions{RequiredInstanceExtensions::get()};

    VkApplicationInfo appInfo{};
    appInfo.sType = vku::GetSType<VkApplicationInfo>();
    appInfo.pNext = nullptr;
    appInfo.pApplicationName = "Teapot";
    appInfo.applicationVersion = 1;
    appInfo.pEngineName = nullptr;
    appInfo.engineVersion = 0;
    appInfo.apiVersion = VK_API_VERSION_1_3;

    std::vector<char const*> requiredExtensionsStr{};
    requiredExtensionsStr.reserve(requiredExtensions.size());

    for (auto const& ext : requiredExtensions) {
        requiredExtensionsStr.push_back(ext.c_str());
    }

    VkInstanceCreateInfo info{};
    info.sType = vku::GetSType<VkInstanceCreateInfo>();
    info.pNext = nullptr;
    info.flags = 0;
    info.pApplicationInfo = &appInfo;
    info.enabledLayerCount = 0;
    info.ppEnabledLayerNames = nullptr;
    info.enabledExtensionCount = static_cast<uint32_t>(requiredExtensionsStr.size());
    info.ppEnabledExtensionNames = requiredExtensionsStr.data();

    VkInstance instance{};
    if (vkCreateInstance(&info, nullptr, &instance) != VK_SUCCESS) {
        return std::unexpected{"failed to create instance"};
    }

    return instance;
}

}  // namespace graphics::vk::impl
