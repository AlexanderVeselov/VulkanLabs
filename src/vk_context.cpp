#include "vk_context.hpp"
#include <iostream>
#include <vector>
#include <stdexcept>

VkContext::VkContext(AppSettings const& settings, char const** req_extensions, std::uint32_t req_extension_count)
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
    create_info.ppEnabledExtensionNames = req_extensions;
    create_info.enabledExtensionCount = req_extension_count;
    create_info.enabledLayerCount = 0;

    if (vkCreateInstance(&create_info, nullptr, &instance_) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create VkInstance!");
    }

    // Get extension count
    uint32_t extension_count = 0;
    if (vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to enumerate instance extension properties!");
    }

    // Get available extensions
    std::vector<VkExtensionProperties> extensions(extension_count);
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());

    // Print extensions
    std::cout << "Available extensions:" << std::endl;
    for (auto const& extension : extensions)
    {
        std::cout << " " << extension.extensionName << std::endl;
    }

    // Get physical device count
    std::uint32_t physical_device_count = 0;
    if (vkEnumeratePhysicalDevices(instance_, &physical_device_count, nullptr) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to enumerate physical devices!");
    }

    // Print physical device information
    std::vector<VkPhysicalDevice> physical_devices(physical_device_count);
    vkEnumeratePhysicalDevices(instance_, &physical_device_count, physical_devices.data());

    for (auto const& device : physical_devices)
    {
        VkPhysicalDeviceProperties device_properties;
        vkGetPhysicalDeviceProperties(device, &device_properties);
        std::cout << "Found physical device:\n" << device_properties.deviceName << std::endl;

    }
}

VkContext::~VkContext()
{
    // Destroy Vulkan instance
    vkDestroyInstance(instance_, nullptr);
}
