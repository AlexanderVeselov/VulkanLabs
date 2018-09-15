#include "vk_device.hpp"
#include "vk_exception.hpp"
#include "vk_validation.hpp"
#include <iostream>
#include <vector>

namespace vklabs
{
    VkDevice::VkDevice(std::size_t id, VkPhysicalDevice physical_device,
        std::vector<char const*> required_extensions)
        : physical_device_(physical_device)
    {
        std::array<std::uint32_t, 2> queue_family_indices =
        {
            GetGraphicsQueueFamilyIndex(),
            GetComputeQueueFamilyIndex()
        };

        std::uint32_t queue_family_count =
            (queue_family_indices[0] == queue_family_indices[1]) ? 1 : 2;

        std::vector<VkDeviceQueueCreateInfo> queue_create_infos(queue_family_count);
        for (std::uint32_t i = 0; i < queue_family_count; ++i)
        {
            queue_create_infos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_create_infos[i].pNext = nullptr;
            queue_create_infos[i].flags = 0;
            queue_create_infos[i].queueFamilyIndex = queue_family_indices[i];
            queue_create_infos[i].queueCount = 1;
            constexpr float queue_priority = 0.0f;
            queue_create_infos[i].pQueuePriorities = &queue_priority;
        }

        VkDeviceCreateInfo device_create_info = {};
        device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        device_create_info.queueCreateInfoCount = static_cast<std::uint32_t>(queue_create_infos.size());
        device_create_info.pQueueCreateInfos = queue_create_infos.data();
        device_create_info.enabledLayerCount = 0;
#if VALIDATION_ENABLED
        device_create_info.enabledLayerCount = static_cast<std::uint32_t>(g_validation_layers.size());
        device_create_info.ppEnabledLayerNames = g_validation_layers.data();
#endif
        device_create_info.enabledExtensionCount = static_cast<std::uint32_t>(required_extensions.size());
        device_create_info.ppEnabledExtensionNames = required_extensions.data();

        ::VkDevice device = nullptr;
        VkResult status = vkCreateDevice(physical_device_, &device_create_info, nullptr, &device);
        VK_THROW_IF_FAILED(status, "Failed to create VkDevice!");

        logical_device_.reset(device, [](::VkDevice device)
        {
            std::cout << "Destroying logical device" << std::endl;
            vkDestroyDevice(device, nullptr);
        });

    }

    std::uint32_t VkDevice::GetGraphicsQueueFamilyIndex() const
    {
        if (graphics_queue_family_index_ == 0xFFFFFFFF)
        {
            std::uint32_t queue_family_count = 0;
           vkGetPhysicalDeviceQueueFamilyProperties(physical_device_, &queue_family_count, nullptr);

            std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
            vkGetPhysicalDeviceQueueFamilyProperties(physical_device_, &queue_family_count, queue_families.data());

            for (std::size_t i = 0; i < queue_families.size(); ++i)
            {
                if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                {
                    graphics_queue_family_index_ = static_cast<std::uint32_t>(i);
                    return graphics_queue_family_index_;
                }
            }

            throw std::runtime_error("Graphics queue family not found!");
        }

        return graphics_queue_family_index_;
    };

    std::uint32_t VkDevice::GetComputeQueueFamilyIndex() const
    {
        if (compute_queue_family_index_ == 0xFFFFFFFF)
        {
            std::uint32_t queue_family_count = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(physical_device_, &queue_family_count, nullptr);

            std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
            vkGetPhysicalDeviceQueueFamilyProperties(physical_device_, &queue_family_count, queue_families.data());

            for (std::size_t i = 0; i < queue_families.size(); ++i)
            {
                if (queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
                {
                    compute_queue_family_index_ = static_cast<std::uint32_t>(i);
                    return compute_queue_family_index_;
                }
            }

            throw std::runtime_error("Compute queue family not found!");
        }

        return compute_queue_family_index_;
    }

    std::uint32_t VkDevice::GetPresentationQueueFamilyIndex(VkSurfaceKHR surface) const
    {
        std::uint32_t queue_family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device_, &queue_family_count, nullptr);

        std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device_, &queue_family_count, queue_families.data());

        for (std::size_t i = 0; i < queue_families.size(); ++i)
        {
            VkBool32 presentation_support = false;
            VkResult status = vkGetPhysicalDeviceSurfaceSupportKHR(physical_device_,
                static_cast<std::uint32_t>(i), surface, &presentation_support);
            VK_THROW_IF_FAILED(status, "Failed to get surface support status!");

            if (presentation_support)
            {
                return static_cast<std::uint32_t>(i);
            }
        }

        throw std::runtime_error("Presentation queue family not found!");
        return 0xFFFFFFFF;

    }

    VkPhysicalDevice VkDevice::GetPhysicalDevice() const
    {
        return physical_device_;
    }

    ::VkDevice VkDevice::GetLogicalDevice() const
    {
        return logical_device_.get();
    }

}
