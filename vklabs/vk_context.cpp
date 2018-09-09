#include "vk_context.hpp"
#include <iostream>
#include <vector>
#include <stdexcept>

namespace vklabs
{
#define THROW_IF_FAILED(err_code, msg) \
    if ((err_code) != VK_SUCCESS)      \
    {                                  \
        throw std::runtime_error(std::string(__FUNCTION__) + ": " + msg); \
    }


    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {

        std::cerr << "[Validation layer]: " << pCallbackData->pMessage << std::endl;

        return VK_FALSE;
    }

    VkResult CreateDebugUtilsMessengerEXT(
        VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkDebugUtilsMessengerEXT* pCallback)
    {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pCallback);
        }
        else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void DestroyDebugUtilsMessengerEXT(
        VkInstance instance,
        VkDebugUtilsMessengerEXT callback,
        const VkAllocationCallbacks* pAllocator)
    {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(instance, callback, pAllocator);
        }
    }

    VkContext::VkContext(AppSettings const& settings, std::vector<char const*> required_extensions)
    {
        // Create basic vulkan instance
        CreateInstance(settings.app_name, required_extensions);

        // Create debug messenger
        if (ValidationEnabled())
        {
            CreateDebugMessenger();
        }

        // Find available extensions
        FindAvailableExtensions();

        // Find physical devices
        FindPhysicalDevices();

        std::uint32_t queue_family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physical_devices_[0], &queue_family_count, nullptr);

        std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(physical_devices_[0], &queue_family_count, queue_families.data());

        for (auto const& prop : queue_families)
        {
            std::cout << prop.queueFlags << ": ";
            if ((prop.queueFlags & VK_QUEUE_GRAPHICS_BIT) == VK_QUEUE_GRAPHICS_BIT)
            {
                std::cout << "VK_QUEUE_GRAPHICS_BIT ";
            }
            if ((prop.queueFlags & VK_QUEUE_COMPUTE_BIT) == VK_QUEUE_COMPUTE_BIT)
            {
                std::cout << "VK_QUEUE_COMPUTE_BIT ";
            }
            if ((prop.queueFlags & VK_QUEUE_TRANSFER_BIT) == VK_QUEUE_TRANSFER_BIT)
            {
                std::cout << "VK_QUEUE_TRANSFER_BIT ";
            }
            if ((prop.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) == VK_QUEUE_SPARSE_BINDING_BIT)
            {
                std::cout << "VK_QUEUE_SPARSE_BINDING_BIT ";
            }
            if ((prop.queueFlags & VK_QUEUE_PROTECTED_BIT) == VK_QUEUE_PROTECTED_BIT)
            {
                std::cout << "VK_QUEUE_PROTECTED_BIT ";
            }
            std::cout << std::endl;
        }

    }

    VkContext::~VkContext()
    {
        if (ValidationEnabled())
        {
            DestroyDebugUtilsMessengerEXT(instance_.get(), debug_messenger_, nullptr);
        }

        // Destroy Vulkan instance
        //vkDestroyInstance(instance_, nullptr);
    }

    void VkContext::CreateInstance(std::string const& application_name,
        std::vector<char const*> required_extensions)
    {
        // Setup VkApplicationInfo
        VkApplicationInfo app_info = {};
        app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app_info.pApplicationName = application_name.c_str();
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
        if (ValidationEnabled())
        {
            required_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        create_info.ppEnabledExtensionNames = required_extensions.data();
        create_info.enabledExtensionCount = static_cast<std::uint32_t>(required_extensions.size());

        VkInstance instance;
        VkResult error_code = vkCreateInstance(&create_info, nullptr, &instance);
        THROW_IF_FAILED(error_code, "Failed to create VkInstance!");

        instance_.reset(instance, [](VkInstance instance)
        {
            std::cout << "Destroying VkInstance" << std::endl;
            vkDestroyInstance(instance, nullptr);
        });
    }

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
        THROW_IF_FAILED(error_code, "Failed to create debug messenger!");

    }

    void VkContext::SetupValidationLayers(VkInstanceCreateInfo& instance_create_info)
    {
        if (ValidationEnabled())
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

            instance_create_info.ppEnabledLayerNames = validation_layers.data();
            instance_create_info.enabledLayerCount = static_cast<std::uint32_t>(validation_layers.size());

        }
        else
        {
            instance_create_info.enabledLayerCount = 0;
        }
    }

    void VkContext::FindAvailableExtensions()
    {
        // Get extension count
        uint32_t extension_count = 0;
        VkResult error_code = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
        THROW_IF_FAILED(error_code, "Failed to enumerate instance extension properties!");

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
        THROW_IF_FAILED(error_code, "Failed to enumerate physical devices!");

        // Print physical device information
        physical_devices_.resize(physical_device_count);
        vkEnumeratePhysicalDevices(instance_.get(), &physical_device_count, physical_devices_.data());

        for (auto const& device : physical_devices_)
        {
            VkPhysicalDeviceProperties device_properties;
            vkGetPhysicalDeviceProperties(device, &device_properties);
            std::cout << "VkContext::VkContext(...): Found physical device:\n" << device_properties.deviceName << std::endl;

        }
    }

}
