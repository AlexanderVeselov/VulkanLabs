#include "vk_device.hpp"
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

    VkDevice::VkDevice(std::uint32_t id, std::vector<char const*> required_extensions,
        VkPhysicalDevice physical_device)
        : physical_device_(physical_device)
    {
        //vkCreateDevice()
    }

    VkDevice::~VkDevice()
    {
    }

}
