#ifndef VKCONTEXT_HPP_
#define VKCONTEXT_HPP_

#include "context.hpp"
#include "app_settings.hpp"
#include <vulkan/vulkan.h>

class VkContext : public Context
{
public:
    VkContext(AppSettings const& settings, char const** req_extensions, std::uint32_t req_extension_count);
    ~VkContext();

private:
    VkInstance instance_ = nullptr;

};

#endif // VKCONTEXT_HPP_
