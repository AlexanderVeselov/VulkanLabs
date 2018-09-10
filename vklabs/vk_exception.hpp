#ifndef VK_EXCEPTION_HPP_
#define VK_EXCEPTION_HPP_

#include <stdexcept>
#include <vulkan/vulkan.h>

inline char* const GetVkErrorString(VkResult error_code)
{
    /*
    VK_SUCCESS = 0,
    VK_NOT_READY = 1,
    VK_TIMEOUT = 2,
    VK_EVENT_SET = 3,
    VK_EVENT_RESET = 4,
    VK_INCOMPLETE = 5,
    VK_ERROR_OUT_OF_HOST_MEMORY = -1,
    VK_ERROR_OUT_OF_DEVICE_MEMORY = -2,
    VK_ERROR_INITIALIZATION_FAILED = -3,
    VK_ERROR_DEVICE_LOST = -4,
    VK_ERROR_MEMORY_MAP_FAILED = -5,
    VK_ERROR_LAYER_NOT_PRESENT = -6,
    VK_ERROR_EXTENSION_NOT_PRESENT = -7,
    VK_ERROR_FEATURE_NOT_PRESENT = -8,
    VK_ERROR_INCOMPATIBLE_DRIVER = -9,
    VK_ERROR_TOO_MANY_OBJECTS = -10,
    VK_ERROR_FORMAT_NOT_SUPPORTED = -11,
    VK_ERROR_FRAGMENTED_POOL = -12,
    VK_ERROR_OUT_OF_POOL_MEMORY = -1000069000,
    VK_ERROR_INVALID_EXTERNAL_HANDLE = -1000072003,
    VK_ERROR_SURFACE_LOST_KHR = -1000000000,
    VK_ERROR_NATIVE_WINDOW_IN_USE_KHR = -1000000001,
    VK_SUBOPTIMAL_KHR = 1000001003,
    VK_ERROR_OUT_OF_DATE_KHR = -1000001004,
    VK_ERROR_INCOMPATIBLE_DISPLAY_KHR = -1000003001,
    VK_ERROR_VALIDATION_FAILED_EXT = -1000011001,
    VK_ERROR_INVALID_SHADER_NV = -1000012000,
    VK_ERROR_FRAGMENTATION_EXT = -1000161000,
    VK_ERROR_NOT_PERMITTED_EXT = -1000174001,
    VK_ERROR_OUT_OF_POOL_MEMORY_KHR = VK_ERROR_OUT_OF_POOL_MEMORY,
    VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR = VK_ERROR_INVALID_EXTERNAL_HANDLE,
    VK_RESULT_BEGIN_RANGE = VK_ERROR_FRAGMENTED_POOL,
    VK_RESULT_END_RANGE = VK_INCOMPLETE,
    VK_RESULT_RANGE_SIZE = (VK_INCOMPLETE - VK_ERROR_FRAGMENTED_POOL + 1),
    VK_RESULT_MAX_ENUM = 0x7FFFFFFF
    */
    switch (error_code)
    {
    case 0: return "VK_SUCCESS";
    case 1: return "VK_NOT_READY";
    case 2: return "VK_TIMEOUT";
    case 3: return "VK_EVENT_SET";
    case 4: return "VK_EVENT_RESET";
    case 5: return "VK_INCOMPLETE";
    case -1: return "VK_ERROR_OUT_OF_HOST_MEMORY";
    case -2: return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
    case -3: return "VK_ERROR_INITIALIZATION_FAILED";
    case -4: return "VK_ERROR_DEVICE_LOST";
    case -5: return "VK_ERROR_MEMORY_MAP_FAILED";
    case -6: return "VK_ERROR_LAYER_NOT_PRESENT";
    case -7: return "VK_ERROR_EXTENSION_NOT_PRESENT";
    case -8: return "VK_ERROR_FEATURE_NOT_PRESENT";
    case -9: return "VK_ERROR_INCOMPATIBLE_DRIVER";
    case -10: return "VK_ERROR_TOO_MANY_OBJECTS";
    case -11: return "VK_ERROR_FORMAT_NOT_SUPPORTED";
    case -12: return "VK_ERROR_FRAGMENTED_POOL";
    case -1000069000: return "VK_ERROR_OUT_OF_POOL_MEMORY";
    case -1000072003: return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
    case -1000000000: return "VK_ERROR_SURFACE_LOST_KHR";
    case -1000000001: return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
    case 1000001003: return "VK_SUBOPTIMAL_KHR";
    case -1000001004: return "VK_ERROR_OUT_OF_DATE_KHR";
    case -1000003001: return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
    case -1000011001: return "VK_ERROR_VALIDATION_FAILED_EXT";
    case -1000012000: return "VK_ERROR_INVALID_SHADER_NV";
    case -1000161000: return "VK_ERROR_FRAGMENTATION_EXT";
    case -1000174001: return "VK_ERROR_NOT_PERMITTED_EXT";
    default: return "Unknown Vulkan Error";
    }
}

class VkException : public std::exception
{
public:
    VkException(std::string const& message, VkResult error_code)
        : std::exception((message + " (" + GetVkErrorString(error_code) + ")").c_str())
    {}
};

#define VK_THROW_IF_FAILED(err_code, msg) \
    if ((err_code) != VK_SUCCESS)      \
    {                                  \
        throw VkException(std::string(__FUNCTION__) + ": " + msg, err_code); \
    }

#endif // VK_EXCEPTION_HPP_
