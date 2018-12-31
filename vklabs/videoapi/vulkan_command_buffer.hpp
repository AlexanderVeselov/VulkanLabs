#ifndef VULKAN_COMMAND_BUFFER_HPP_
#define VULKAN_COMMAND_BUFFER_HPP_

#include "vulkan_device.hpp"
#include <vulkan/vulkan.h>

class VulkanCommandBuffer
{
public:
    VulkanCommandBuffer(VulkanDevice & device, VkCommandPool command_pool);

private:
    VulkanDevice & device_;

    VkCommandPool command_pool_;
    VkCommandBuffer command_buffer_;

};

#endif // VULKAN_COMMAND_BUFFER_HPP_
