#ifndef VKDEVICE_HPP_
#define VKDEVICE_HPP_

#include "device.hpp"
#include "vk_shared_object.hpp"
#include <vulkan/vulkan.h>
#include <vector>
#include <map>

namespace vklabs
{
    class VkDevice : public Device
    {
    public:
        VkDevice(std::size_t id, VkPhysicalDevice physical_device,
            std::vector<char const*> required_extensions);

        std::uint32_t GetGraphicsQueueFamilyIndex() const;

        std::uint32_t GetComputeQueueFamilyIndex() const;

    private:
        void FindQueueFamilyIndices();

    private:
        VkPhysicalDevice physical_device_;
        VkSharedObject<::VkDevice> logical_device_;
        std::map<VkQueueFlags, std::uint32_t> queue_family_indices_;

    };

}

#endif // VKDEVICE_HPP_
