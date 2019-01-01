#include "vulkan_shader.hpp"
#include "vulkan_exception.hpp"
#include <vector>
#include <fstream>

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

    VkShaderModuleCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    create_info.codeSize = shader_code.size();
    create_info.pCode = (std::uint32_t const*)shader_code.data();

    VkResult status = shader_module_.Create(logical_device, create_info);
    VK_THROW_IF_FAILED(status, "Failed to create shader module!");

}

VkShaderModule VulkanShader::GetShaderModule() const
{
    return shader_module_;
}
