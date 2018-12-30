#ifndef VULKAN_API_HPP_
#define VULKAN_API_HPP_

#include "videoapi/video_api.hpp"
#include "videoapi/vulkan_shared_object.hpp"
#include "videoapi/vulkan_validation.hpp"
#include <vector>

class VulkanDevice;

class VulkanAPI : public VideoAPI
{
public:
    VulkanAPI(std::vector<char const*> const& enabled_extensions, bool enable_validation, std::uint32_t device_index);
    ~VulkanAPI();

    VulkanSharedObject<VkInstance> GetInstance() const;

private:
    void CreateInstance(std::vector<char const*> enabled_extensions);
    void CreateDebugMessenger();
    void CreateDevice(std::vector<char const*> const& enabled_extensions, std::uint32_t physical_device_index);

private:
    bool validation_enabled_;
    VulkanSharedObject<VkInstance> instance_;
    VkDebugUtilsMessengerEXT debug_messenger_ = nullptr;
    std::shared_ptr<VulkanDevice> device_;

};


#endif // VULKAN_API_HPP_
