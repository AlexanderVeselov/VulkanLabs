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

    VkDevice::VkDevice(std::size_t id, VkPhysicalDevice physical_device,
        std::vector<char const*> required_extensions)
        : physical_device_(physical_device)
    {

        std::uint32_t queue_family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device_, &queue_family_count, nullptr);

        std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device_, &queue_family_count, queue_families.data());

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

    VkDevice::~VkDevice()
    {
    }

}
