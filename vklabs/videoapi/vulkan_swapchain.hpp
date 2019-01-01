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
    std::uint32_t GetImagesCount() const { return swapchain_images_.size(); }
    VkImageView GetImageView(std::uint32_t index) const { return swapchain_image_views_[index]; }
    std::uint32_t GetCurrentImageIndex() const { return current_image_index_; }
    void Present();

private:
    VulkanDevice & device_;
    VulkanScopedObject<VkSwapchainKHR, vkCreateSwapchainKHR, vkDestroySwapchainKHR> swapchain_;
    std::vector<VkImage> swapchain_images_;
    std::vector<VulkanScopedObject<VkImageView, vkCreateImageView, vkDestroyImageView>> swapchain_image_views_;
    VkFormat swapchain_image_format_;
    std::uint32_t current_image_index_;
    VkQueue present_queue_;

};

#endif // VULKAN_SWAPCHAIN_HPP_
