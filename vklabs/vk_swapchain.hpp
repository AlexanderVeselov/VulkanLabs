#ifndef VK_SWAPCHAIN_HPP_
#define VK_SWAPCHAIN_HPP_

#include "swapchain.hpp"
#include "vk_shared_object.hpp"
#include "vk_device.hpp"
#include <vulkan/vulkan.h>
#include <vector>

namespace vklabs
{
    class VkSwapchain : public Swapchain
    {
    public:
        VkSwapchain(std::shared_ptr<Device> device, VkSharedObject<VkSurfaceKHR> surface,
            std::uint32_t width, std::uint32_t height);
        void Present() const override;

    private:
        VkSurfaceFormatKHR FindSurfaceFormat() const;
        VkPresentModeKHR FindPresentationMode() const;

        std::shared_ptr<VkDevice> device_;
        VkSharedObject<VkSurfaceKHR> surface_;
        VkSharedObject<VkSwapchainKHR> swapchain_;
        std::vector<VkImage> swapchain_images_;
        VkFormat swapchain_image_format_;

    };

}
#endif // VK_SWAPCHAIN_HPP_
