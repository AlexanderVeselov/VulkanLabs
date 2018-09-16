#ifndef VK_SHADER_HPP_
#define VK_SHADER_HPP_

#include "shader.hpp"
#include "vk_shared_object.hpp"
#include <vulkan/vulkan.h>

class VkShader : public Shader
{
public:

private:
    VkSharedObject<VkShaderModule> shader_module_;
};

#endif // VK_SHADER_HPP_
