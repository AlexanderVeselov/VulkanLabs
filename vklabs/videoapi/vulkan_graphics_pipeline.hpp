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
    VkRenderPass GetRenderPass() const { return render_pass_.get(); }
    VkPipeline GetPipeline() const { return pipeline_.get(); }
    VkFramebuffer GetFramebuffer() const { return framebuffer_.get(); }
    VkExtent2D GetExtent() const { return extent_; }

private:
    VulkanDevice & device_;

    std::shared_ptr<VulkanShader> vertex_shader_;
    std::shared_ptr<VulkanShader> pixel_shader_;

    VulkanSharedObject<VkPipelineLayout> pipeline_layout_;
    VulkanSharedObject<VkRenderPass> render_pass_;
    VulkanSharedObject<VkPipeline> pipeline_;
    VulkanSharedObject<VkFramebuffer> framebuffer_;
    VkExtent2D extent_;

};

#endif // VULKAN_GRAPHICS_PIPELINE_HPP_
