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

private:
    void SetupValidationLayers(VkInstanceCreateInfo& create_info);

private:
    VkInstance instance_ = nullptr;
    VkDebugUtilsMessengerEXT debug_messenger_ = nullptr;
#ifdef NDEBUG
    static const bool enable_validation_ = false;
#else
    static const bool enable_validation_ = true;
#endif

};

#endif // VKCONTEXT_HPP_
