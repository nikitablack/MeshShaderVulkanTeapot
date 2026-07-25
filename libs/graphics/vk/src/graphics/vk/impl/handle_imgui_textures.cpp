#include <imgui.h>

#include <graphics/vk/FrameData.hpp>
#include <graphics/vk/impl/handle_imgui_textures.hpp>
#include <graphics/vk/utils/init_helper.hpp>
#include <optional>
#include <span>
#include <utils/try_expected.hpp>

namespace {

auto find_image(VkImage image, std::vector<graphics::vk::Image2d> const& allImages)
    -> std::optional<graphics::vk::Image2d> {
    for (auto const& img : allImages) {
        if (img.image() == image) {
            return img;
        }
    }

    return std::nullopt;
}

}  // namespace

namespace graphics::vk::impl {

auto handle_imgui_textures(VkDevice device,  //
                           VmaAllocator allocator,  //
                           VkCommandPool commandPool,  //
                           VkQueue queue,  //
                           std::vector<Image2d>& imguiImagesInOut  //
                           ) noexcept -> std::expected<void, std::string> {
    auto const* const drawData{ImGui::GetDrawData()};

    if (drawData->Textures) {
        for (auto* const tex : *drawData->Textures) {
            switch (tex->Status) {
                case ImTextureStatus::ImTextureStatus_WantCreate: {
                    if (tex->Format != ImTextureFormat::ImTextureFormat_RGBA32) {
                        return std::unexpected{"unsupported imgui image format"};
                    }

                    Image2d imguiImage{};

                    TRY_EXPECTED_VOID(imguiImage.init(allocator,  //
                                                      device,  //
                                                      VK_FORMAT_R8G8B8A8_SRGB,  //
                                                      VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,  //
                                                      VK_IMAGE_ASPECT_COLOR_BIT,  //
                                                      static_cast<uint32_t>(tex->Width),  //
                                                      static_cast<uint32_t>(tex->Height)));

                    imguiImagesInOut.push_back(imguiImage);

                    std::span<std::byte const> texData{std::span{static_cast<std::byte const*>(tex->GetPixels()),  //
                                                                 static_cast<size_t>(tex->GetSizeInBytes())}};

                    utils::init_image_sync(device,  //
                                           allocator,  //
                                           commandPool,  //
                                           imguiImage,  //
                                           texData,  //
                                           VK_PIPELINE_STAGE_2_NONE,  //
                                           VK_ACCESS_2_NONE,  //
                                           VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT,  //
                                           VK_ACCESS_2_SHADER_READ_BIT,  //
                                           VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,  //
                                           queue);

                    tex->SetTexID(imguiImage.view());
                    tex->SetStatus(ImTextureStatus::ImTextureStatus_OK);
                    tex->BackendUserData = imguiImage.image();
                } break;

                case ImTextureStatus::ImTextureStatus_WantUpdates: {
                    auto const vkImage{static_cast<VkImage>(tex->BackendUserData)};
                    auto imguiImage{find_image(vkImage, imguiImagesInOut)};

                    if (!imguiImage) {
                        return std::unexpected{"failed to find imgui image"};
                    }

                    std::span<std::byte const> texData{std::span{static_cast<std::byte const*>(tex->GetPixels()),  //
                                                                 static_cast<size_t>(tex->GetSizeInBytes())}};

                    utils::init_image_sync(device,  //
                                           allocator,  //
                                           commandPool,  //
                                           *imguiImage,  //
                                           texData,  //
                                           VK_PIPELINE_STAGE_2_NONE,  //
                                           VK_ACCESS_2_NONE,  //
                                           VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT,  //
                                           VK_ACCESS_2_SHADER_READ_BIT,  //
                                           VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,  //
                                           queue);

                    tex->SetStatus(ImTextureStatus::ImTextureStatus_OK);
                } break;

                case ImTextureStatus::ImTextureStatus_WantDestroy: {
                    auto const vkImage{static_cast<VkImage>(tex->BackendUserData)};
                    auto imguiImage{find_image(vkImage, imguiImagesInOut)};

                    if (!imguiImage) {
                        return std::unexpected{"failed to find imgui image"};
                    }

                    vkQueueWaitIdle(queue);

                    std::erase_if(imguiImagesInOut,
                                  [&](Image2d const& img) { return imguiImage->image() == img.image(); });

                    imguiImage->destroy();

                    tex->SetStatus(ImTextureStatus::ImTextureStatus_Destroyed);
                    tex->BackendUserData = nullptr;
                    tex->SetTexID(VK_NULL_HANDLE);
                } break;

                default:
                    // do nothing
                    break;
            }
        }
    }

    return {};
}

}  // namespace graphics::vk::impl
