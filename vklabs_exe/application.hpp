#ifndef APPLICATION_HPP_
#define APPLICATION_HPP_

#include "vk_context.hpp"
#include "app_settings.hpp"
#include <GLFW/glfw3.h>
#include <memory>

namespace vklabs
{
    class Application
    {
    public:
        Application(AppSettings const& settings);
        ~Application();
        void Run();

    private:
        AppSettings settings_;
        std::unique_ptr<GLFWwindow, void(*)(GLFWwindow*)> window_;
        std::shared_ptr<VkContext> context_;
        std::shared_ptr<Device> device_;
        std::shared_ptr<Swapchain> swapchain_;
        VkSharedObject<VkSurfaceKHR> surface_;


    };

}

#endif // APPLICATION_HPP_
