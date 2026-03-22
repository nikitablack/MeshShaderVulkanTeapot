#include <fmt/core.h>
#include <vulkan/vulkan.h>

#include <graphics/vk/impl/RequiredApiVersion.hpp>
#include <graphics/vk/impl/check_instance_version.hpp>

namespace graphics::vk::impl {

[[nodiscard]] auto check_instance_version() noexcept -> std::expected<void, std::string> {
    fmt::println("checking instance version");

    auto const f{
        reinterpret_cast<PFN_vkEnumerateInstanceVersion>(vkGetInstanceProcAddr(nullptr, "vkEnumerateInstanceVersion"))};

    if (!f) {
        return std::unexpected{"detected Vulkan version is < 1.1"};
    }

    uint32_t apiVersion{};
    if (vkEnumerateInstanceVersion(&apiVersion) != VK_SUCCESS) {
        return std::unexpected{"failed to enumerate instance version"};
    }

    uint32_t const major{VK_API_VERSION_MAJOR(apiVersion)};
    uint32_t const minor{VK_API_VERSION_MINOR(apiVersion)};
    uint32_t const patch{VK_API_VERSION_PATCH(apiVersion)};

    if ((major < RequiredApiVersion::MAJOR) ||
        (major >= RequiredApiVersion::MAJOR && minor < RequiredApiVersion::MINOR)) {
        return std::unexpected{
            fmt::format("minimum required Vulkan api version: {}.{}.0\ndetected Vulkan api version: {}.{}.{}",
                        RequiredApiVersion::MAJOR, RequiredApiVersion::MINOR, major, minor, patch)};
    }

    fmt::println("Vulkan version: {}.{}.{}", major, minor, patch);

    return {};
}

}  // namespace graphics::vk::impl