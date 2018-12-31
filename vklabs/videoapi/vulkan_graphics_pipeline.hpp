#ifndef VULKAN_GRAPHICS_PIPELINE_HPP_
#define VULKAN_GRAPHICS_PIPELINE_HPP_

#include "vulkan_shared_object.hpp"
#include "vulkan_shader.hpp"
#include "vulkan_device.hpp"
#include <vulkan/vulkan.h>
#include <memory>

class VulkanGraphicsPipeline
{
public:
    VulkanGraphicsPipeline(VulkanDevice & device, std::shared_ptr<VulkanShader> vertex_shader, std::shared_ptr<VulkanShader> pixel_shader);

private:
    VulkanDevice & device_;

    std::shared_ptr<VulkanShader> vertex_shader_;
    std::shared_ptr<VulkanShader> pixel_shader_;

    VulkanSharedObject<VkPipelineLayout> pipeline_layout_;
    VulkanSharedObject<VkRenderPass> render_pass_;
    VulkanSharedObject<VkPipeline> pipeline_;

};

#endif // VULKAN_GRAPHICS_PIPELINE_HPP_
