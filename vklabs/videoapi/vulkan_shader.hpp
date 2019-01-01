#ifndef VULKAN_SHADER_HPP_
#define VULKAN_SHADER_HPP_

#include "vulkan_device.hpp"
#include "vulkan_shared_object.hpp"
#include <string>
#include <vulkan/vulkan.h>

class VulkanShader
{
public:
    VulkanShader(VulkanDevice & device, std::string const& filename);
    VkShaderModule GetShaderModule() const;

private:
    VulkanDevice & device_;
    VulkanScopedObject<VkShaderModule, vkCreateShaderModule, vkDestroyShaderModule> shader_module_;

};

#endif // VULKAN_SHADER_HPP_