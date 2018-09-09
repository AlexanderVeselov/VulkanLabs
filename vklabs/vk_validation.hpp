#ifndef VK_VALIDATION_HPP_
#define VK_VALIDATION_HPP_

#ifdef NDEBUG
#define VALIDATION_ENABLED 0
#else
#define VALIDATION_ENABLED 1
#endif

#include <array>
#include <iostream>
#include <vulkan/vulkan.h>

static const std::array<char const*, 1> g_validation_layers =
{
#if VALIDATION_ENABLED
    "VK_LAYER_LUNARG_standard_validation"
#endif
};

#if VALIDATION_ENABLED

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData)
{

    std::cerr << "[Validation layer]: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}

inline VkResult CreateDebugUtilsMessengerEXT(
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

inline void DestroyDebugUtilsMessengerEXT(
    VkInstance instance,
    VkDebugUtilsMessengerEXT callback,
    const VkAllocationCallbacks* pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, callback, pAllocator);
    }
}
#endif

#endif // VK_VALIDATION_HPP