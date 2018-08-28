#ifndef VKCONTEXT_HPP_
#define VKCONTEXT_HPP_

#include "context.hpp"
#include "app_settings.hpp"
#include <vulkan/vulkan.h>
#include <vector>

class VkContext : public Context
{
public:
    VkContext(AppSettings const& settings, std::vector<char const*> required_extensions);
    ~VkContext();

    bool ValidationEnabled() const { return enable_validation_; }

private:
    void CreateInstance(std::string const& application_name, std::vector<char const*> required_extensions);
    void CreateDebugMessenger();
    void SetupValidationLayers(VkInstanceCreateInfo& create_info);
    void FindAvailableExtensions();
    void FindPhysicalDevices();

private:
    VkInstance instance_ = nullptr;
    VkDebugUtilsMessengerEXT debug_messenger_ = nullptr;
    std::vector<VkExtensionProperties> available_extensions_;
    std::vector<VkPhysicalDevice> physical_devices_;
#ifdef NDEBUG
    static const bool enable_validation_ = false;
#else
    static const bool enable_validation_ = true;
#endif

};

#endif // VKCONTEXT_HPP_
