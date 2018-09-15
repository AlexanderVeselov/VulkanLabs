#include "vk_context.hpp"
#include "vk_device.hpp"
#include "vk_swapchain.hpp"
#include "vk_exception.hpp"
#include <iostream>
#include <vector>

namespace vklabs
{
    VkContext::VkContext(std::vector<char const*> const& required_extensions)
    {
        // Create basic vulkan instance
        CreateInstance(required_extensions);

#if VALIDATION_ENABLED
        // Create debug messenger
        CreateDebugMessenger();
#endif

        // Find available extensions
        FindAvailableExtensions();

        // Find physical devices
        FindPhysicalDevices();

    }

    VkContext::~VkContext()
    {
#if VALIDATION_ENABLED
        DestroyDebugUtilsMessengerEXT(instance_.get(), debug_messenger_, nullptr);
#endif
    }

    void VkContext::CreateInstance(std::vector<char const*> required_extensions)
    {
        // Setup VkApplicationInfo
        VkApplicationInfo app_info = {};
        app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app_info.pApplicationName = "CGLabs";
        app_info.applicationVersion = 1;
        app_info.pEngineName = "No Engine";
        app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.apiVersion = VK_API_VERSION_1_0;

        // Create Vulkan instance

        VkInstanceCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        create_info.pApplicationInfo = &app_info;
        // CHECK: What does The Standard say about
        // struct initialization with empty braces {}?
        create_info.enabledLayerCount = 0;

#if VALIDATION_ENABLED
        SetupValidationLayers(create_info);
        // Add extension that required by validation layers message callback
        required_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

        create_info.ppEnabledExtensionNames = required_extensions.data();
        create_info.enabledExtensionCount = static_cast<std::uint32_t>(required_extensions.size());

        VkInstance instance;
        VkResult error_code = vkCreateInstance(&create_info, nullptr, &instance);
        VK_THROW_IF_FAILED(error_code, "Failed to create VkInstance!");

        instance_.reset(instance, [](VkInstance instance)
        {
            std::cout << "Destroying VkInstance" << std::endl;
            vkDestroyInstance(instance, nullptr);
        });
    }

#if VALIDATION_ENABLED
    void VkContext::CreateDebugMessenger()
    {
        // Create debug utils messenger
        VkDebugUtilsMessengerCreateInfoEXT messenger_create_info = {};
        messenger_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

        messenger_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

        messenger_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

        messenger_create_info.pfnUserCallback = DebugCallback;
        messenger_create_info.pUserData = nullptr;

        VkResult error_code = CreateDebugUtilsMessengerEXT(instance_.get(), &messenger_create_info, nullptr, &debug_messenger_);
        VK_THROW_IF_FAILED(error_code, "Failed to create debug messenger!");

    }

    void VkContext::SetupValidationLayers(VkInstanceCreateInfo& instance_create_info)
    {
        std::uint32_t layer_count;
        vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

        std::vector<VkLayerProperties> available_layers(layer_count);
        vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());
        for (auto const& required_layer : g_validation_layers)
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

        instance_create_info.ppEnabledLayerNames = g_validation_layers.data();
        instance_create_info.enabledLayerCount = static_cast<std::uint32_t>(g_validation_layers.size());

    }
#endif

    void VkContext::FindAvailableExtensions()
    {
        // Get extension count
        uint32_t extension_count = 0;
        VkResult error_code = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
        VK_THROW_IF_FAILED(error_code, "Failed to enumerate instance extension properties!");

        // Get available extensions
        available_extensions_.resize(extension_count);
        vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, available_extensions_.data());

        // Print extensions
        std::cout << "VkContext::VkContext(...): Available extensions:" << std::endl;
        for (auto const& extension : available_extensions_)
        {
            std::cout << " " << extension.extensionName << std::endl;
        }
    }

    void VkContext::FindPhysicalDevices()
    {
        // Get physical device count
        std::uint32_t physical_device_count = 0;
        VkResult error_code = vkEnumeratePhysicalDevices(instance_.get(), &physical_device_count, nullptr);
        VK_THROW_IF_FAILED(error_code, "Failed to enumerate physical devices!");

        // Print physical device information
        physical_devices_.resize(physical_device_count);
        vkEnumeratePhysicalDevices(instance_.get(), &physical_device_count, physical_devices_.data());

        for (auto const& device : physical_devices_)
        {
            VkPhysicalDeviceProperties device_properties;
            vkGetPhysicalDeviceProperties(device, &device_properties);
            std::cout << "VkContext::VkContext(...): Found physical device:\n"
                << device_properties.deviceName << std::endl;

        }
    }

    std::shared_ptr<Device> VkContext::CreateDevice(std::size_t index,
        std::vector<char const*> const& required_extensions) const
    {
        return std::make_shared<VkDevice>(index, physical_devices_[index], required_extensions);
    }

    std::shared_ptr<Swapchain> VkContext::CreateSwapchain(std::shared_ptr<Device> device,
        VkSurfaceKHR surface, std::uint32_t width, std::uint32_t height) const
    {
        VkInstance instance = instance_.get();
        VkSharedObject<VkSurfaceKHR> surf(surface, [instance](VkSurfaceKHR surface)
        {
            std::cout << "Destroying VkSurfaceKHR" << std::endl;
            vkDestroySurfaceKHR(instance, surface, nullptr);
        });

        return std::make_shared<VkSwapchain>(device, surf, width, height);
    }

    VkInstance VkContext::GetInstance() const
    {
        return instance_.get();
    }

}
