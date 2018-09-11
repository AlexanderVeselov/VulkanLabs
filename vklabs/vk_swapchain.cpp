#include "vk_swapchain.hpp"

VkSwapchain::VkSwapchain(VkSurfaceKHR surface)
{
    surface_.reset(surface,
        [](VkSurfaceKHR)
    {
        // TODO: Get instance here
        //vkDestroySurfaceKHR()
    });
}
