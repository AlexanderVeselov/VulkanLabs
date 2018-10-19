#ifndef VULKAN_API_HPP_
#define VULKAN_API_HPP_

#include "videoapi/video_api.hpp"
#include "videoapi/vulkan_validation.hpp"
#include "videoapi/vulkan_shared_object.hpp"
#include <vector>

class VulkanDevice;

class VulkanAPI : public VideoAPI
{
public:
    VulkanAPI(std::vector<char const*> const& required_extensions, std::uint32_t device_index);
    ~VulkanAPI();
    
    VulkanSharedObject<VkInstance> GetInstance() const;

private:
    void CreateInstance(std::vector<char const*> required_extensions);
#if VALIDATION_ENABLED
    void CreateDebugMessenger();
    void SetupValidationLayers(VkInstanceCreateInfo& create_info);
#endif
    void FindAvailableExtensions();
    void FindPhysicalDevices();

private:
    VulkanSharedObject<VkInstance> instance_;
    VkDebugUtilsMessengerEXT debug_messenger_ = nullptr;
    std::vector<VkExtensionProperties> available_extensions_;
    std::vector<VkPhysicalDevice> physical_devices_;
    std::shared_ptr<VulkanDevice> device_;

};


#endif // VULKAN_API_HPP_
