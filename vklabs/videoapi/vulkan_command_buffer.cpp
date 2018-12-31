#include "vulkan_command_buffer.hpp"
#include "vulkan_exception.hpp"

VulkanCommandBuffer::VulkanCommandBuffer(VulkanDevice & device, VkCommandPool command_pool)
    : device_(device)
    , command_pool_(command_pool)
{
    VkCommandBufferAllocateInfo allocate_info = {};
    allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocate_info.commandPool = command_pool_;
    allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocate_info.commandBufferCount = 1;

    VkDevice logical_device = device_.GetDevice();
    VkResult status = vkAllocateCommandBuffers(logical_device, &allocate_info, &command_buffer_);
    VK_THROW_IF_FAILED(status, "Failed to allocate command buffer!");

}
