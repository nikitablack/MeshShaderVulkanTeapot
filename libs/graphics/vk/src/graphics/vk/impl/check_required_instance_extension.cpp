#include <fmt/core.h>
#include <vulkan/vulkan.h>

#include <graphics/vk/impl/RequiredInstanceExtensions.hpp>
#include <graphics/vk/impl/check_required_instance_extension.hpp>

namespace graphics::vk::impl {

auto check_required_instance_extension() noexcept -> std::expected<void, std::string> {
    fmt::println("checking required instance extensions");

    RequiredInstanceExtensions::print();

    uint32_t extensionCount{};
    if (vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr) != VK_SUCCESS) {
        return std::unexpected{"failed to enumerate instance extension properties"};
    }

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    if (vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data()) != VK_SUCCESS) {
        return std::unexpected{"failed to enumerate instance extension properties"};
    }

    for (auto const& reqExt : RequiredInstanceExtensions::get()) {
        for (auto const& avExt : availableExtensions) {
            if (reqExt == avExt.extensionName) {
                goto cnt;
            }
        }

        return std::unexpected{"instance extension \"{}\" is not supported"};

    cnt:;
    }

    return {};
}

}  // namespace graphics::vk::impl