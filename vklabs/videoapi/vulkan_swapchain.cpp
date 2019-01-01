#include "vulkan_exception.hpp"
#include "vulkan_swapchain.hpp"
#include "vulkan_device.hpp"
#include <algorithm>

static VkSurfaceFormatKHR FindSurfaceFormat(VkPhysicalDevice physical_device, VkSurfaceKHR surface)
{
    std::vector<VkSurfaceFormatKHR> available_formats;

    std::uint32_t format_count;
    VkResult status = vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, nullptr);
    VK_THROW_IF_FAILED(status, "Failed to get physical device surface formats!");

    available_formats.resize(format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, available_formats.data());

    THROW_IF(available_formats.empty(), "No available surface formats!");

    if (available_formats.size() == 1 && available_formats[0].format == VK_FORMAT_UNDEFINED)
    {
        return { VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
    }

    for (const auto& available_format : available_formats)
    {
        if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB
            && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return available_format;
        }
    }

    return available_formats[0];

}

VkPresentModeKHR FindPresentMode(VkPhysicalDevice physical_device, VkSurfaceKHR surface)
{
    std::vector<VkPresentModeKHR> available_present_modes;
    std::uint32_t presentation_mode_count;
    VkResult status = vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &presentation_mode_count, nullptr);
    VK_THROW_IF_FAILED(status, "Failed to get physical device presentation modes!");

    available_present_modes.resize(presentation_mode_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &presentation_mode_count, available_present_modes.data());

    THROW_IF(available_present_modes.empty(), "No available presentation modes!");

    VkPresentModeKHR best_mode = VK_PRESENT_MODE_FIFO_KHR;

    for (const auto& available_present_mode : available_present_modes)
    {
        if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return available_present_mode;
        }
        else if (available_present_mode == VK_PRESENT_MODE_IMMEDIATE_KHR)
        {
            best_mode = available_present_mode;
        }

    }

    return best_mode;
}

VulkanSwapchain::VulkanSwapchain(VulkanDevice & device, std::uint32_t width, std::uint32_t height)
    : device_(device)
{
    VkSurfaceCapabilitiesKHR surface_capabilities;

    VkSurfaceKHR surface = device_.GetSurface();

    VkResult status = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device_.GetPhysicalDevice(),
        surface, &surface_capabilities);
    VK_THROW_IF_FAILED(status, "Failed to get surface capabilities!");

    std::uint32_t image_count = surface_capabilities.minImageCount + 1;

    if (surface_capabilities.maxImageCount > 0 && image_count > surface_capabilities.maxImageCount)
    {
        image_count = surface_capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR swapchain_create_info = {};
    swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchain_create_info.surface = surface;
    swapchain_create_info.minImageCount = image_count;

    VkSurfaceFormatKHR surface_format = FindSurfaceFormat(device_.GetPhysicalDevice(), surface);
    swapchain_create_info.imageFormat = surface_format.format;
    swapchain_create_info.imageColorSpace = surface_format.colorSpace;

    VkExtent2D image_extent;

    if (surface_capabilities.currentExtent.width != 0xFFFFFFFF)
    {
        image_extent = surface_capabilities.currentExtent;
    }
    else
    {
        image_extent = { width, height };

        image_extent.width = std::max(surface_capabilities.minImageExtent.width,
            std::min(surface_capabilities.maxImageExtent.width, image_extent.width));
        image_extent.height = std::max(surface_capabilities.minImageExtent.height,
            std::min(surface_capabilities.maxImageExtent.height, image_extent.height));

    }

    swapchain_create_info.imageExtent = image_extent;
    swapchain_create_info.imageArrayLayers = 1;
    swapchain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    std::uint32_t graphics_family_index = device_.GetGraphicsQueueFamilyIndex();
    std::uint32_t present_family_index = device_.GetPresentQueueFamilyIndex();

    std::uint32_t queue_family_indices[] =
    {
        graphics_family_index,
        present_family_index
    };

    if (graphics_family_index == present_family_index)
    {
        swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }
    else
    {
        swapchain_create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchain_create_info.queueFamilyIndexCount = 2;
        swapchain_create_info.pQueueFamilyIndices = queue_family_indices;
    }

    swapchain_create_info.preTransform = surface_capabilities.currentTransform;
    swapchain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchain_create_info.presentMode = FindPresentMode(device_.GetPhysicalDevice(), surface);
    swapchain_create_info.clipped = VK_TRUE;

    swapchain_create_info.oldSwapchain = nullptr;

    VkSwapchainKHR swapchain = nullptr;
    VkDevice logical_device = device_.GetDevice();
    status = vkCreateSwapchainKHR(logical_device, &swapchain_create_info, nullptr, &swapchain);
    VK_THROW_IF_FAILED(status, "Failed to create swap chain!");

    swapchain_.reset(swapchain, [logical_device](VkSwapchainKHR swapchain)
    {
        vkDestroySwapchainKHR(logical_device, swapchain, nullptr);
    });

    status = vkGetSwapchainImagesKHR(logical_device, swapchain, &image_count, nullptr);
    VK_THROW_IF_FAILED(status, "Failed to get swap chain images!");
    THROW_IF(image_count == 0, "No swap chain images!");

    swapchain_images_.resize(image_count);
    vkGetSwapchainImagesKHR(logical_device, swapchain, &image_count, swapchain_images_.data());

    // Create swapchain image views
    swapchain_image_views_.resize(swapchain_images_.size());

    for (std::size_t i = 0; i < swapchain_images_.size(); i++)
    {
        VkImageViewCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        create_info.image = swapchain_images_[i];
        create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        create_info.format = surface_format.format;
        create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        create_info.subresourceRange.baseMipLevel = 0;
        create_info.subresourceRange.levelCount = 1;
        create_info.subresourceRange.baseArrayLayer = 0;
        create_info.subresourceRange.layerCount = 1;

        VkImageView image_view;
        status = vkCreateImageView(logical_device, &create_info, nullptr, &image_view);
        VK_THROW_IF_FAILED(status, "Failed to create image view!");

        swapchain_image_views_[i].reset(image_view, [logical_device](VkImageView image_view)
        {
            vkDestroyImageView(logical_device, image_view, nullptr);
        });


    }

    status = vkAcquireNextImageKHR(logical_device, swapchain_.get(), std::numeric_limits<uint64_t>::max(), VK_NULL_HANDLE, VK_NULL_HANDLE, &current_image_index_);
    VK_THROW_IF_FAILED(status, "Failed to acquire next image!");

    vkGetDeviceQueue(logical_device, device_.GetPresentQueueFamilyIndex(), 0, &present_queue_);
}

void VulkanSwapchain::Present()
{
    VkPresentInfoKHR present_info = {};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.swapchainCount = 1;

    VkSwapchainKHR swapchain = swapchain_.get();
    present_info.pSwapchains = &swapchain;

    present_info.pImageIndices = &current_image_index_;

    VkResult status = vkQueuePresentKHR(present_queue_, &present_info);
    VK_THROW_IF_FAILED(status, "Failed to present image!");

    status = vkAcquireNextImageKHR(device_.GetDevice(), swapchain_.get(), std::numeric_limits<uint64_t>::max(), VK_NULL_HANDLE, VK_NULL_HANDLE, &current_image_index_);
    VK_THROW_IF_FAILED(status, "Failed to acquire next image!");

}
