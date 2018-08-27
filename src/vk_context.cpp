#include "vk_context.hpp"
#include <iostream>
#include <vector>
#include <stdexcept>

#define THROW_IF_FAILED(err_code, msg) \
    if ((err_code) != VK_SUCCESS)      \
    {                                  \
        throw std::runtime_error(std::string(__FUNCTION__) + ": " + msg); \
    }

VkContext::VkContext(AppSettings const& settings, std::vector<char const*> required_extensions)
{
    // Setup VkApplicationInfo
    VkApplicationInfo app_info = {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = settings.app_name.c_str();
    app_info.applicationVersion = 1;
    app_info.pEngineName = "No Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_0;

    // Create Vulkan instance
    VkInstanceCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;
    SetupValidationLayers(create_info);
    
    // Add extension that required by validation layers message callback
    if constexpr (enable_validation_)
    {
        required_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    create_info.ppEnabledExtensionNames = required_extensions.data();
    create_info.enabledExtensionCount = static_cast<std::uint32_t>(required_extensions.size());

    VkResult error_code;
    error_code = vkCreateInstance(&create_info, nullptr, &instance_);
    THROW_IF_FAILED(error_code, "Failed to create VkInstance!");

    // Get extension count
    uint32_t extension_count = 0;
    error_code = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
    THROW_IF_FAILED(error_code, "Failed to enumerate instance extension properties!");

    // Get available extensions
    std::vector<VkExtensionProperties> extensions(extension_count);
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());

    // Print extensions
    std::cout << "VkContext::VkContext(...): Available extensions:" << std::endl;
    for (auto const& extension : extensions)
    {
        std::cout << " " << extension.extensionName << std::endl;
    }

    // Get physical device count
    std::uint32_t physical_device_count = 0;
    error_code = vkEnumeratePhysicalDevices(instance_, &physical_device_count, nullptr);
    THROW_IF_FAILED(error_code, "Failed to enumerate physical devices!");

    // Print physical device information
    std::vector<VkPhysicalDevice> physical_devices(physical_device_count);
    vkEnumeratePhysicalDevices(instance_, &physical_device_count, physical_devices.data());

    for (auto const& device : physical_devices)
    {
        VkPhysicalDeviceProperties device_properties;
        vkGetPhysicalDeviceProperties(device, &device_properties);
        std::cout << "VkContext::VkContext(...): Found physical device:\n" << device_properties.deviceName << std::endl;

    }
}

VkContext::~VkContext()
{
    // Destroy Vulkan instance
    vkDestroyInstance(instance_, nullptr);
}

void VkContext::SetupValidationLayers(VkInstanceCreateInfo& create_info)
{
    if constexpr (enable_validation_)
    {
        static const std::vector<char const*> validation_layers =
        {
            "VK_LAYER_LUNARG_standard_validation"
        };

        std::uint32_t layer_count;
        vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

        std::vector<VkLayerProperties> available_layers(layer_count);
        vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());
        for (auto const& required_layer : validation_layers)
        {
            bool layer_found = false;
            for (auto const& available_layer : available_layers)
            {
                if (strcmp(required_layer, available_layer.layerName) == 0)
                {
                    layer_found = true;
                    break;
                }
            }

            if (!layer_found)
            {
                throw std::runtime_error("VkContext::SetupValidationLayers(...): required layer '"
                    + std::string(required_layer) + "'is not available");
            }
        }

        create_info.ppEnabledLayerNames = validation_layers.data();
        create_info.enabledLayerCount = static_cast<std::uint32_t>(validation_layers.size());

    }
    else
    {
        create_info.enabledLayerCount = 0;
    }
}
