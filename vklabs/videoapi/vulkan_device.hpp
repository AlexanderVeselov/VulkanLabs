#ifndef VULKAN_DEVICE_HPP_
#define VULKAN_DEVICE_HPP_

#include "vulkan_shared_object.hpp"
#include <vulkan/vulkan.h>
#include <vector>

class VulkanDevice
{
public:
    VulkanDevice(VkPhysicalDevice physical_device, std::vector<char const*> const& enabled_layer_names, std::vector<char const*> const& enabled_extension_names);

    std::uint32_t GetGraphicsQueueFamilyIndex() const;
    std::uint32_t GetComputeQueueFamilyIndex() const;
    std::uint32_t GetTransferQueueFamilyIndex() const;
    std::uint32_t GetPresentQueueFamilyIndex() const;

    VkPhysicalDevice GetPhysicalDevice() const;
    VulkanSharedObject<VkDevice> GetLogicalDevice() const;

private:
    void FindQueueFamilyIndices();
    void CreateLogicalDevice(std::vector<char const*> const& enabled_layer_names, std::vector<char const*> const& enabled_extension_names);

    VkPhysicalDevice physical_device_;
    VulkanSharedObject<VkDevice> logical_device_;
    VkSurfaceKHR surface_;

    std::uint32_t graphics_queue_family_index_;
    std::uint32_t compute_queue_family_index_;
    std::uint32_t transfer_queue_family_index_;
    std::uint32_t present_queue_family_index_;

};

#endif // VULKAN_DEVICE_HPP_
