#include "vulkan_shader.hpp"
#include "vulkan_exception.hpp"
#include <vector>
#include <fstream>

static VkShaderModule CreateShaderModule(VkDevice device, std::vector<char> const& code)
{
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = (std::uint32_t const*)code.data();

    VkShaderModule shaderModule;
    VkResult status = vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule);
    VK_THROW_IF_FAILED(status, "Failed to create shader module!");

    return shaderModule;
}

static void ReadShaderCodeFromFile(std::string const& filename, std::vector<char> & code)
{
    std::ifstream input_file(filename, std::ios::in | std::ios::ate | std::ios::binary);

    THROW_IF(!input_file, "Failed to open shader file!");

    std::streampos file_size = input_file.tellg();
    code.resize(file_size);
    input_file.seekg(0);

    input_file.read(code.data(), file_size);

}

VulkanShader::VulkanShader(VulkanDevice & device, std::string const& filename)
    : device_(device)
{
    std::vector<char> shader_code;
    ReadShaderCodeFromFile(filename, shader_code);
    VkDevice logical_device = device_.GetDevice();

    VkShaderModule shader_module = CreateShaderModule(logical_device, shader_code);
    shader_module_.reset(shader_module, [logical_device](VkShaderModule shader_module)
    {
        vkDestroyShaderModule(logical_device, shader_module, nullptr);
    });

}

VkShaderModule VulkanShader::GetShaderModule() const
{
    return shader_module_.get();
}
