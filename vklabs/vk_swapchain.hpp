#ifndef VK_SWAPCHAIN_HPP_
#define VK_SWAPCHAIN_HPP_

#include "swapchain.hpp"
#include "vk_shared_object.hpp"
#include <vulkan/vulkan.h>

class VkSwapchain
{
public:
    VkSwapchain(VkSurfaceKHR surface);
private:
    VkSharedObject<VkSurfaceKHR> surface_;
};

#endif // VK_SWAPCHAIN_HPP_
