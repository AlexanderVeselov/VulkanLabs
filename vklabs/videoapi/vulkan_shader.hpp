#ifndef VULKAN_SHADER_HPP_
#define VULKAN_SHADER_HPP_

#include "vulkan_device.hpp"
#include "vulkan_shared_object.hpp"
#include <string>
#include <vulkan/vulkan.h>

class VulkanShader
{
public:
    enum ShaderType
    {
        kVertex,
        kPixel
    };

    VulkanShader(VulkanDevice & device, ShaderType shader_type, std::string const& filename);
    VkShaderModule GetShaderModule() const;
    std::vector<VkVertexInputAttributeDescription> const& GetVertexInputAttributeDescriptions() const { return vertex_input_attribute_descs_; }
    std::vector<VkVertexInputBindingDescription> const& GetVertexInputBindingDescriptions() const { return vertex_input_binding_desc_; }

private:
    VulkanDevice & device_;
    ShaderType shader_type_;
    VulkanScopedObject<VkShaderModule, vkCreateShaderModule, vkDestroyShaderModule> shader_module_;
    std::vector<VkVertexInputAttributeDescription> vertex_input_attribute_descs_;
    std::vector<VkVertexInputBindingDescription> vertex_input_binding_desc_;

};

#endif // VULKAN_SHADER_HPP_