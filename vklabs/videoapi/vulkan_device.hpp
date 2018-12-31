#ifndef VULKAN_DEVICE_HPP_
#define VULKAN_DEVICE_HPP_

#include "vulkan_shared_object.hpp"
#include "videoapi/vulkan_api.hpp"
#include <vulkan/vulkan.h>
#include <vector>

class VulkanSwapchain;
class VulkanShader;
class VulkanGraphicsPipeline;

class VulkanDevice
{
public:
    VulkanDevice(VulkanAPI & video_api, VkPhysicalDevice physical_device, std::vector<char const*> const& enabled_layer_names, std::vector<char const*> const& enabled_extension_names, VulkanSharedObject<VkSurfaceKHR> surface);

    std::uint32_t GetGraphicsQueueFamilyIndex() const;
    std::uint32_t GetComputeQueueFamilyIndex() const;
    std::uint32_t GetTransferQueueFamilyIndex() const;
    std::uint32_t GetPresentQueueFamilyIndex() const;

    VkPhysicalDevice GetPhysicalDevice() const;
    VkDevice GetDevice() const;
    VkSurfaceKHR GetSurface() const;

    std::shared_ptr<VulkanSwapchain> CreateSwapchain(std::uint32_t width, std::uint32_t height);
    std::shared_ptr<VulkanShader> CreateShader(std::string const& filename);
    std::shared_ptr<VulkanGraphicsPipeline> CreateGraphicsPipeline(std::shared_ptr<VulkanShader> vertex_shader, std::shared_ptr<VulkanShader> pixel_shader);

private:
    VulkanAPI & video_api_;

    void FindQueueFamilyIndices();
    void CreateLogicalDevice(std::vector<char const*> const& enabled_layer_names, std::vector<char const*> const& enabled_extension_names);
    void CreateCommandPools();

    VkPhysicalDevice physical_device_;
    VulkanSharedObject<VkDevice> logical_device_;
    VulkanSharedObject<VkSurfaceKHR> surface_;

    std::uint32_t graphics_queue_family_index_;
    std::uint32_t compute_queue_family_index_;
    std::uint32_t transfer_queue_family_index_;
    std::uint32_t present_queue_family_index_;

    VulkanSharedObject<VkCommandPool> graphics_command_pool_;
    VulkanSharedObject<VkCommandPool> compute_command_pool_;
    VulkanSharedObject<VkCommandPool> transfer_command_pool_;

};

#endif // VULKAN_DEVICE_HPP_
