#include <fmt/core.h>

#include <graphics/vk/impl/RequiredApiVersion.hpp>
#include <graphics/vk/impl/RequiredDeviceExtensions.hpp>
#include <graphics/vk/impl/check_physical_device_support.hpp>
#include <graphics/vk/impl/features/RequiredFeatures.hpp>
#include <graphics/vk/impl/get_physical_device_properties.hpp>
#include <utils/try_expected.hpp>
#include <vector>

namespace {

auto check_required_device_extensions(VkPhysicalDevice device) noexcept -> std::expected<void, std::string> {
    uint32_t extensionCount{};
    if (vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr) != VK_SUCCESS) {
        return std::unexpected{"failed to enumerate physical device extension properties"};
    }

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    if (vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data()) !=
        VK_SUCCESS) {
        return std::unexpected{"failed to enumerate physical device extension properties"};
    }

    for (auto const& reqExtension : graphics::vk::impl::RequiredDeviceExtensions::get()) {
        for (auto const& extension : availableExtensions) {
            if (reqExtension == extension.extensionName) {
                goto cnt;
            }
        }

        return std::unexpected{"required device extension \"" + reqExtension + "\" is not supported"};

    cnt:;
    }

    return {};
}

}  // namespace

namespace graphics::vk::impl {

[[nodiscard]] auto check_physical_device_support(VkPhysicalDevice device) noexcept -> std::expected<void, std::string> {
    const auto properties{get_physical_device_properties(device)};

    fmt::println("checking physical device support: {}", properties.properties.deviceName);

    const uint32_t major{VK_API_VERSION_MAJOR(properties.properties.apiVersion)};
    const uint32_t minor{VK_API_VERSION_MINOR(properties.properties.apiVersion)};
    const uint32_t patch{VK_API_VERSION_PATCH(properties.properties.apiVersion)};

    if ((major < RequiredApiVersion::MAJOR) ||
        ((major == RequiredApiVersion::MAJOR) && (minor < RequiredApiVersion::MINOR))) {
        return std::unexpected{fmt::format("minimum supported Vulkan version: {}.{}.0\ndevice's api version: {}.{}.{}",
                                           RequiredApiVersion::MAJOR, RequiredApiVersion::MINOR, major, minor, patch)};
    }

    if (!features::RequiredFeatures::check(device)) {
        return std::unexpected{"required features are not supported"};
    }

    TRY_EXPECTED_VOID(check_required_device_extensions(device));

    return {};
}

}  // namespace graphics::vk::impl