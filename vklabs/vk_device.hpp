#ifndef VKDEVICE_HPP_
#define VKDEVICE_HPP_

#include "device.hpp"
#include "vk_shared_object.hpp"
#include "vk_swapchain.hpp"
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>

namespace vklabs
{
    class VkDevice : public Device
    {
    public:
        VkDevice(std::size_t id, VkPhysicalDevice physical_device,
            std::vector<char const*> required_extensions);

        std::uint32_t GetGraphicsQueueFamilyIndex() const;
        std::uint32_t GetComputeQueueFamilyIndex() const;
        std::uint32_t GetPresentationQueueFamilyIndex(VkSurfaceKHR surface) const;

    public:
        VkPhysicalDevice GetPhysicalDevice() const;
        ::VkDevice GetLogicalDevice() const;

    private:
        VkPhysicalDevice physical_device_;
        VkSharedObject<::VkDevice> logical_device_;
        mutable std::uint32_t graphics_queue_family_index_ = 0xFFFFFFFF;
        mutable std::uint32_t compute_queue_family_index_ = 0xFFFFFFFF;

    };

}

#endif // VKDEVICE_HPP_
