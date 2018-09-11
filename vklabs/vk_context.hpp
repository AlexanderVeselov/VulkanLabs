#ifndef VKCONTEXT_HPP_
#define VKCONTEXT_HPP_

#include "context.hpp"
#include "app_settings.hpp"
#include "vk_shared_object.hpp"
#include "vk_validation.hpp"
#include <vulkan/vulkan.h>
#include <vector>

namespace vklabs
{
    class VkContext : public Context
    {
    public:
        VkContext(std::vector<char const*> const& required_extensions);
        ~VkContext();

        std::shared_ptr<Device> CreateDevice(std::size_t index,
            std::vector<char const*> const& required_extensions) const override;

        std::shared_ptr<Swapchain> CreateSwapchain() const;

    private:
        void CreateInstance(std::vector<char const*> required_extensions);
#if VALIDATION_ENABLED
        void CreateDebugMessenger();
        void SetupValidationLayers(VkInstanceCreateInfo& create_info);
#endif
        void FindAvailableExtensions();
        void FindPhysicalDevices();

    private:
        VkSharedObject<VkInstance> instance_;
        VkDebugUtilsMessengerEXT debug_messenger_ = nullptr;
        std::vector<VkExtensionProperties> available_extensions_;
        std::vector<VkPhysicalDevice> physical_devices_;

    };

}

#endif // VKCONTEXT_HPP_
