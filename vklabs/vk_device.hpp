#ifndef VKDEVICE_HPP_
#define VKDEVICE_HPP_

#include "device.hpp"
#include <vulkan/vulkan.h>
#include <vector>

namespace vklabs
{
    class VkDevice : public Device
    {
    public:
        VkDevice(std::size_t id, VkPhysicalDevice physical_device,
            std::vector<char const*> required_extensions);
        ~VkDevice();

    private:
        VkPhysicalDevice physical_device_;
    };

}

#endif // VKDEVICE_HPP_
