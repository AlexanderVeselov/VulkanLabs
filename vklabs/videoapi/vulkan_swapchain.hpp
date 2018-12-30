#ifndef VULKAN_SWAPCHAIN_HPP_
#define VULKAN_SWAPCHAIN_HPP_

#include "videoapi/vulkan_shared_object.hpp"
#include "videoapi/vulkan_device.hpp"
#include <vulkan/vulkan.h>
#include <vector>

class VulkanSwapchain 
{
public:
    VulkanSwapchain(VulkanDevice & device, std::uint32_t width, std::uint32_t height);
    //void Present() const override;

private:
    VulkanDevice & device_;
    VulkanSharedObject<VkSwapchainKHR> swapchain_;
    std::vector<VkImage> swapchain_images_;
    //std::vector<VkSharedObject<VkImageView>> swapchain_image_views_;
    VkFormat swapchain_image_format_;

};

#endif // VULKAN_SWAPCHAIN_HPP_
