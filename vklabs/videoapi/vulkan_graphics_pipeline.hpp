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
    VulkanGraphicsPipeline(VulkanDevice & device, std::shared_ptr<VulkanShader> vertex_shader, std::shared_ptr<VulkanShader> pixel_shader, std::uint32_t width, std::uint32_t height, VkImageView attachment);
    VkRenderPass GetRenderPass() const { return render_pass_; }
    VkPipeline GetPipeline() const { return pipeline_; }
    VkFramebuffer GetFramebuffer() const { return framebuffer_; }
    VkExtent2D GetExtent() const { return extent_; }

private:
    VulkanDevice & device_;

    std::shared_ptr<VulkanShader> vertex_shader_;
    std::shared_ptr<VulkanShader> pixel_shader_;

    VulkanScopedObject<VkPipelineLayout, vkCreatePipelineLayout, vkDestroyPipelineLayout> pipeline_layout_;
    VulkanScopedObject<VkRenderPass, vkCreateRenderPass, vkDestroyRenderPass> render_pass_;
    VulkanScopedObject<VkPipeline, vkCreateGraphicsPipelines, vkDestroyPipeline> pipeline_;
    VulkanScopedObject<VkFramebuffer, vkCreateFramebuffer, vkDestroyFramebuffer>  framebuffer_;
    VkExtent2D extent_;

};

#endif // VULKAN_GRAPHICS_PIPELINE_HPP_
