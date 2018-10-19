#ifndef VULKAN_DEVICE_HPP_
#define VULKAN_DEVICE_HPP_

#include "vulkan_shared_object.hpp"
#include <vulkan/vulkan.h>
#include <vector>

class VulkanDevice
{
public:
    VulkanDevice(VkPhysicalDevice physical_device, std::vector<char const*> const& required_extensions);

    std::uint32_t GetGraphicsQueueFamilyIndex() const;
    std::uint32_t GetComputeQueueFamilyIndex() const;
    std::uint32_t GetPresentationQueueFamilyIndex(VkSurfaceKHR surface) const;

    VkPhysicalDevice GetPhysicalDevice() const;
    VulkanSharedObject<VkDevice> GetLogicalDevice() const;

private:
    VkPhysicalDevice physical_device_;
    VulkanSharedObject<VkDevice> logical_device_;
    mutable std::uint32_t graphics_queue_family_index_ = 0xFFFFFFFF;
    mutable std::uint32_t compute_queue_family_index_ = 0xFFFFFFFF;

};

#endif // VULKAN_DEVICE_HPP_
