#include <fmt/core.h>

#include <graphics/vk/impl/RequiredDeviceExtensions.hpp>
#include <graphics/vk/impl/check_physical_device_support.hpp>
#include <graphics/vk/impl/features/RequiredFeatures.hpp>
#include <graphics/vk/impl/get_physical_device_properties.hpp>
#include <graphics/vk/impl/get_supported_physical_devices.hpp>

namespace graphics::vk::impl {

[[nodiscard]] auto get_supported_physical_devices(VkInstance instance  //
                                                  ) noexcept
    -> std::expected<std::vector<VkPhysicalDevice>, std::string> {
    fmt::println("getting physical devices");

    RequiredDeviceExtensions::print();
    features::RequiredFeatures::print();

    uint32_t deviceCount{};
    if ((vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr) != VK_SUCCESS) || (deviceCount == 0)) {
        return std::unexpected{"failed to find GPUs with Vulkan support"};
    }

    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    if (vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices.data()) != VK_SUCCESS) {
        return std::unexpected{"failed to find GPUs with Vulkan support"};
    }

    std::vector<VkPhysicalDevice> supportedPhysicalDevices{};
    supportedPhysicalDevices.reserve(deviceCount);

    for (auto const device : physicalDevices) {
        auto const properties{get_physical_device_properties(device)};

        if (auto const result{check_physical_device_support(device)}) {
            supportedPhysicalDevices.push_back(device);
            fmt::println("\t{} is supported", properties.properties.deviceName);
        } else {
            fmt::println("\t{} is not supported: \n\t{}", properties.properties.deviceName, result.error());
        }
    }

    if (supportedPhysicalDevices.empty()) {
        return std::unexpected{"failed to find supported devices"};
    }

    fmt::println("supported devices");

    for (size_t i{0}; i < supportedPhysicalDevices.size(); ++i) {
        auto const properties{get_physical_device_properties(supportedPhysicalDevices[i])};

        fmt::println("\t{}: {}", i, properties.properties.deviceName);
    }

    return supportedPhysicalDevices;
}

}  // namespace graphics::vk::impl
