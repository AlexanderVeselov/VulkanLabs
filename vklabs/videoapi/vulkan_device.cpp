#include "vulkan_device.hpp"
#include "vulkan_exception.hpp"
#include "vulkan_validation.hpp"
#include "vulkan_swapchain.hpp"
#include "vulkan_shader.hpp"
#include "vulkan_graphics_pipeline.hpp"
#include <vector>
#include <algorithm>
#include <cassert>

namespace
{
    const std::uint32_t kInvalidQueueFamilyIndex = ~0u;
}

VulkanDevice::VulkanDevice(VulkanAPI & video_api,
                           VkPhysicalDevice physical_device,
                           std::vector<char const*> const& enabled_layer_names,
                           std::vector<char const*> const& enabled_extension_names,
                           VulkanSharedObject<VkSurfaceKHR> surface)
    : video_api_(video_api)
    , physical_device_(physical_device)
    , surface_(surface)
    , graphics_queue_family_index_(kInvalidQueueFamilyIndex)
    , compute_queue_family_index_(kInvalidQueueFamilyIndex)
    , transfer_queue_family_index_(kInvalidQueueFamilyIndex)
    , present_queue_family_index_(kInvalidQueueFamilyIndex)
{
    // Find queue family indices without retrieval present queue family index
    FindQueueFamilyIndices();
    CreateLogicalDevice(enabled_layer_names, enabled_extension_names);

}

void VulkanDevice::FindQueueFamilyIndices()
{
    // Get physical device queue families count
    std::uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device_, &queue_family_count, nullptr);

    // Get queue families properties
    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device_, &queue_family_count, queue_families.data());

    // Get graphics, compute and transfer queue family indices
    for (std::uint32_t i = 0; i < queue_families.size(); ++i)
    {
        if (graphics_queue_family_index_ == kInvalidQueueFamilyIndex && queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            graphics_queue_family_index_ = i;
        }

        if (compute_queue_family_index_ == kInvalidQueueFamilyIndex && queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
        {
            compute_queue_family_index_ = i;
        }

        if (transfer_queue_family_index_ == kInvalidQueueFamilyIndex && queue_families[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
        {
            transfer_queue_family_index_ = i;
        }

        if (surface_ && present_queue_family_index_ == kInvalidQueueFamilyIndex)
        {
            VkBool32 present_support = false;
            VkResult status = vkGetPhysicalDeviceSurfaceSupportKHR(physical_device_,
                i, surface_.get(), &present_support);
            VK_THROW_IF_FAILED(status, "Failed to get surface support status!");

            if (present_support)
            {
                present_queue_family_index_ = i;
            }
        }

    }

    // Checks
    if (graphics_queue_family_index_ == kInvalidQueueFamilyIndex)
    {
        throw std::runtime_error("VulkanDevice::FindQueueFamilyIndices(): Failed to find graphics queue family index!");
    }

    if (compute_queue_family_index_ == kInvalidQueueFamilyIndex)
    {
        throw std::runtime_error("VulkanDevice::FindQueueFamilyIndices(): Failed to get compute queue family index!");
    }

    if (transfer_queue_family_index_ == kInvalidQueueFamilyIndex)
    {
        throw std::runtime_error("VulkanDevice::FindQueueFamilyIndices(): Failed to get transfer queue family index!");
    }

    if (surface_ && present_queue_family_index_ == kInvalidQueueFamilyIndex)
    {
        throw std::runtime_error("VulkanDevice::FindQueueFamilyIndices(): Failed to get present queue family index!");
    }

}

void VulkanDevice::CreateLogicalDevice(std::vector<char const*> const& enabled_layer_names, std::vector<char const*> const& enabled_extension_names)
{
    std::vector<std::uint32_t> queue_family_indices;
    std::vector<std::uint32_t> queue_counts;
    queue_family_indices.push_back(graphics_queue_family_index_);
    // CHECK: graphics queue count
    queue_counts.push_back(1);

    if (compute_queue_family_index_!= graphics_queue_family_index_)
    {
        queue_family_indices.push_back(compute_queue_family_index_);
        // CHECK: compute queue count
        queue_counts.push_back(1);
    }

    if (transfer_queue_family_index_ != graphics_queue_family_index_ && transfer_queue_family_index_ != compute_queue_family_index_)
    {
        queue_family_indices.push_back(transfer_queue_family_index_);
        // CHECK: transfer queue count
        queue_counts.push_back(1);
    }

    if (surface_)
    {
        if (present_queue_family_index_ != graphics_queue_family_index_ && present_queue_family_index_ != compute_queue_family_index_ && present_queue_family_index_ != transfer_queue_family_index_)
        {
            queue_family_indices.push_back(present_queue_family_index_);
            // One present queue
            queue_counts.push_back(1);
        }
    }

    std::vector<VkDeviceQueueCreateInfo> queue_create_infos(queue_family_indices.size());
    std::vector<float> queue_priorities(*std::max_element(queue_counts.begin(), queue_counts.end()), 1.0f);
    for (std::uint32_t i = 0; i < queue_create_infos.size(); ++i)
    {
        queue_create_infos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_infos[i].pNext = nullptr;
        queue_create_infos[i].flags = 0;
        queue_create_infos[i].queueFamilyIndex = queue_family_indices[i];
        queue_create_infos[i].queueCount = queue_counts[i];
        queue_create_infos[i].pQueuePriorities = queue_priorities.data();
    }

    VkDeviceCreateInfo device_create_info = {};
    device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    device_create_info.queueCreateInfoCount = static_cast<std::uint32_t>(queue_create_infos.size());
    device_create_info.pQueueCreateInfos = queue_create_infos.data();

    device_create_info.enabledLayerCount = static_cast<std::uint32_t>(enabled_layer_names.size());
    device_create_info.ppEnabledLayerNames = enabled_layer_names.data();

    device_create_info.enabledExtensionCount = static_cast<std::uint32_t>(enabled_extension_names.size());
    device_create_info.ppEnabledExtensionNames = enabled_extension_names.data();

    VkDevice device = nullptr;
    VkResult status = vkCreateDevice(physical_device_, &device_create_info, nullptr, &device);
    VK_THROW_IF_FAILED(status, "Failed to create VkDevice!");

    logical_device_.reset(device, [](::VkDevice device)
    {
        std::cout << "Destroying logical device" << std::endl;
        vkDestroyDevice(device, nullptr);
    });

}

std::uint32_t VulkanDevice::GetGraphicsQueueFamilyIndex() const
{
    return graphics_queue_family_index_;
};

std::uint32_t VulkanDevice::GetComputeQueueFamilyIndex() const
{
    return compute_queue_family_index_;
}

std::uint32_t VulkanDevice::GetTransferQueueFamilyIndex() const
{
    return transfer_queue_family_index_;
}

std::uint32_t VulkanDevice::GetPresentQueueFamilyIndex() const
{
    return present_queue_family_index_;
}

VkPhysicalDevice VulkanDevice::GetPhysicalDevice() const
{
    return physical_device_;
}

VkDevice VulkanDevice::GetDevice() const
{
    return logical_device_.get();
}

VkSurfaceKHR VulkanDevice::GetSurface() const
{
    return surface_.get();
}

std::shared_ptr<VulkanSwapchain> VulkanDevice::CreateSwapchain(std::uint32_t width, std::uint32_t height)
{
    assert(surface_);
    return std::make_shared<VulkanSwapchain>(std::ref(*this), width, height);
}

std::shared_ptr<VulkanShader> VulkanDevice::CreateShader(std::string const& filename)
{
    return std::make_shared<VulkanShader>(*this, filename);
}

std::shared_ptr<VulkanGraphicsPipeline> VulkanDevice::CreateGraphicsPipeline(std::shared_ptr<VulkanShader> vertex_shader, std::shared_ptr<VulkanShader> pixel_shader)
{
    return std::make_shared<VulkanGraphicsPipeline>(*this, vertex_shader, pixel_shader);
}
