#include "vulkan_shader.hpp"
#include "vulkan_exception.hpp"
#include "spirv_cross.hpp"
#include <vector>
#include <map>
#include <fstream>

static VkFormat SPIRTypeToVkFormat(spirv_cross::SPIRType const& spv_type)
{
    switch (spv_type.basetype)
    {
    case spirv_cross::SPIRType::Float:
        switch (spv_type.vecsize)
        {
        case 1:
            return VK_FORMAT_R32_SFLOAT;
        case 2:
            return VK_FORMAT_R32G32_SFLOAT;
        case 3:
            return VK_FORMAT_R32G32B32_SFLOAT;
        case 4:
            return VK_FORMAT_R32G32B32A32_SFLOAT;
        }
    case spirv_cross::SPIRType::Double:
        switch (spv_type.vecsize)
        {
        case 1:
            return VK_FORMAT_R64_SFLOAT;
        case 2:
            return VK_FORMAT_R64G64_SFLOAT;
        case 3:
            return VK_FORMAT_R64G64B64_SFLOAT;
        case 4:
            return VK_FORMAT_R64G64B64A64_SFLOAT;
        }
    case spirv_cross::SPIRType::Half:
        switch (spv_type.vecsize)
        {
        case 1:
            return VK_FORMAT_R16_SFLOAT;
        case 2:
            return VK_FORMAT_R16G16_SFLOAT;
        case 3:
            return VK_FORMAT_R16G16B16_SFLOAT;
        case 4:
            return VK_FORMAT_R16G16B16A16_SFLOAT;
        }
    case spirv_cross::SPIRType::Int:
        switch (spv_type.vecsize)
        {
        case 1:
            return VK_FORMAT_R32_SINT;
        case 2:
            return VK_FORMAT_R32G32_SINT;
        case 3:
            return VK_FORMAT_R32G32B32_SINT;
        case 4:
            return VK_FORMAT_R32G32B32A32_SINT;
        }
    case spirv_cross::SPIRType::UInt:
        switch (spv_type.vecsize)
        {
        case 1:
            return VK_FORMAT_R32_UINT;
        case 2:
            return VK_FORMAT_R32G32_UINT;
        case 3:
            return VK_FORMAT_R32G32B32_UINT;
        case 4:
            return VK_FORMAT_R32G32B32A32_UINT;
        }
    case spirv_cross::SPIRType::SByte:
        switch (spv_type.vecsize)
        {
        case 1:
            return VK_FORMAT_R8_SINT;
        case 2:
            return VK_FORMAT_R8G8_SINT;
        case 3:
            return VK_FORMAT_R8G8B8_SINT;
        case 4:
            return VK_FORMAT_R8G8B8A8_SINT;
        }
    case spirv_cross::SPIRType::UByte:
        switch (spv_type.vecsize)
        {
        case 1:
            return VK_FORMAT_R8_UINT;
        case 2:
            return VK_FORMAT_R8G8_UINT;
        case 3:
            return VK_FORMAT_R8G8B8_UINT;
        case 4:
            return VK_FORMAT_R8G8B8A8_UINT;
        }
    case spirv_cross::SPIRType::Short:
        switch (spv_type.vecsize)
        {
        case 1:
            return VK_FORMAT_R16_SINT;
        case 2:
            return VK_FORMAT_R16G16_SINT;
        case 3:
            return VK_FORMAT_R16G16B16_SINT;
        case 4:
            return VK_FORMAT_R16G16B16A16_SINT;
        }
    case spirv_cross::SPIRType::UShort:
        switch (spv_type.vecsize)
        {
        case 1:
            return VK_FORMAT_R16_UINT;
        case 2:
            return VK_FORMAT_R16G16_UINT;
        case 3:
            return VK_FORMAT_R16G16B16_UINT;
        case 4:
            return VK_FORMAT_R16G16B16A16_UINT;
        }
    case spirv_cross::SPIRType::Int64:
        switch (spv_type.vecsize)
        {
        case 1:
            return VK_FORMAT_R64_SINT;
        case 2:
            return VK_FORMAT_R64G64_SINT;
        case 3:
            return VK_FORMAT_R64G64B64_SINT;
        case 4:
            return VK_FORMAT_R64G64B64A64_SINT;
        }
    case spirv_cross::SPIRType::UInt64:
        switch (spv_type.vecsize)
        {
        case 1:
            return VK_FORMAT_R64_UINT;
        case 2:
            return VK_FORMAT_R64G64_UINT;
        case 3:
            return VK_FORMAT_R64G64B64_UINT;
        case 4:
            return VK_FORMAT_R64G64B64A64_UINT;
        }
    default:
        throw std::runtime_error("Failed to convert SPIRType::BaseType to VkFormat!");
    }
}

static VkShaderStageFlags ShaderTypeToVkShaderStageFlags(VulkanShader::ShaderType shader_type)
{
    switch (shader_type)
    {
    case VulkanShader::kVertex:
        return VK_SHADER_STAGE_VERTEX_BIT;
    case VulkanShader::kPixel:
        return VK_SHADER_STAGE_FRAGMENT_BIT;
    }
}

static void ReadShaderCodeFromFile(std::string const& filename, std::vector<std::uint32_t> & code)
{
    std::ifstream input_file(filename, std::ios::in | std::ios::ate | std::ios::binary);

    THROW_IF(!input_file, "Failed to open shader file!");

    std::streampos file_size = input_file.tellg();
    assert(file_size % sizeof(std::uint32_t) == 0);

    code.resize(file_size / sizeof(std::uint32_t));
    input_file.seekg(0);

    input_file.read((char*)code.data(), file_size);

}

VulkanShader::VulkanShader(VulkanDevice & device, ShaderType shader_type, std::string const& filename)
    : device_(device)
    , shader_type_(shader_type)
{
    std::vector<std::uint32_t> shader_code;
    ReadShaderCodeFromFile(filename, shader_code);
    VkDevice logical_device = device_.GetDevice();

    VkShaderModuleCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    create_info.codeSize = shader_code.size() * sizeof(std::uint32_t);
    create_info.pCode = shader_code.data();

    VkResult status = shader_module_.Create(logical_device, create_info);
    VK_THROW_IF_FAILED(status, "Failed to create shader module!");

    spirv_cross::Compiler compiler(std::move(shader_code));
    spirv_cross::ShaderResources resources = compiler.get_shader_resources();

    if (shader_type_ == kVertex)
    {
        FillVertexInputDescriptions(compiler, resources);
    }

    FillBindings(compiler, resources);
}

void VulkanShader::FillVertexInputDescriptions(spirv_cross::Compiler const& compiler, spirv_cross::ShaderResources const& resources)
{
    vertex_input_attribute_descs_.resize(resources.stage_inputs.size());
    std::uint32_t current_offset = 0;
    for (std::size_t i = 0; i < resources.stage_inputs.size(); ++i)
    {
        spirv_cross::Resource const& resource = resources.stage_inputs[i];
        vertex_input_attribute_descs_[i].binding = 0;
        vertex_input_attribute_descs_[i].location = compiler.get_decoration(resource.id, spv::DecorationLocation);
        spirv_cross::SPIRType const& resource_type = compiler.get_type(resource.type_id);
        vertex_input_attribute_descs_[i].format = SPIRTypeToVkFormat(resource_type);
        vertex_input_attribute_descs_[i].offset = current_offset;
        current_offset += resource_type.width / 8 * resource_type.vecsize;
    }

    VkVertexInputBindingDescription vertex_binding_desc = {};
    vertex_binding_desc.binding = 0;
    vertex_binding_desc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    vertex_binding_desc.stride = current_offset;
    vertex_input_binding_desc_.push_back(vertex_binding_desc);
}

void VulkanShader::FillBindings(spirv_cross::Compiler const& compiler, spirv_cross::ShaderResources const& resources)
{
    std::vector<VkDescriptorSetLayoutBinding> bindings;

    VkDescriptorSetLayoutBinding binding;
    // TODO: array support?
    binding.descriptorCount = 1;
    binding.stageFlags = ShaderTypeToVkShaderStageFlags(shader_type_);
    binding.pImmutableSamplers = nullptr;

    for (spirv_cross::Resource const& resource : resources.uniform_buffers)
    {
        binding.binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
        binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        bindings.push_back(binding);
    }

    for (spirv_cross::Resource const& resource : resources.storage_buffers)
    {
        binding.binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
        binding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        bindings.push_back(binding);
    }

    for (spirv_cross::Resource const& resource : resources.storage_images)
    {
        binding.binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
        binding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        bindings.push_back(binding);
    }

    for (spirv_cross::Resource const& resource : resources.sampled_images)
    {
        binding.binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
        binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        bindings.push_back(binding);
    }

    for (spirv_cross::Resource const& resource : resources.separate_images)
    {
        binding.binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
        binding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
        bindings.push_back(binding);
    }

    for (spirv_cross::Resource const& resource : resources.separate_samplers)
    {
        binding.binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
        binding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
        bindings.push_back(binding);
    }

    VkDescriptorSetLayoutCreateInfo desc_set_layout_create_info = {};
    desc_set_layout_create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    desc_set_layout_create_info.bindingCount = static_cast<std::uint32_t>(bindings.size());
    desc_set_layout_create_info.pBindings = bindings.data();
    //vkDestroyDescriptorSetLayout
    //vkCreateDescriptorSetLayout(device_.GetDevice(), )

}

VkShaderModule VulkanShader::GetShaderModule() const
{
    return shader_module_;
}
