#include "vk_context.hpp"
#include "vk_device.hpp"
#include "application.hpp"
#include "vk_exception.hpp"
#include <stdexcept>
#include <iostream>
#include <vector>
#include <vulkan/vulkan.h>

namespace vklabs
{
    Application::Application(AppSettings const& settings)
        : settings_(settings)
        , window_(nullptr, glfwDestroyWindow)
        , context_(nullptr)
    {
        if (!glfwInit())
        {
            throw std::runtime_error("glfwInit() failed");
        }

        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        // Create main window
        window_.reset(glfwCreateWindow(settings.width, settings.height,
            settings_.app_name.c_str(), nullptr, nullptr));

        if (!window_.get())
        {
            throw std::runtime_error("Failed to create GLFW window!");
        }
        
        // Get GLFW requested extensions
        std::uint32_t glfw_extension_count;
        char const** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
        std::vector<char const*> required_extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

        context_.reset(new VkContext(required_extensions));

        const std::vector<const char*> device_extensions =
        {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        device_ = context_->CreateDevice(0, device_extensions);
        VkSurfaceKHR surface;
        VkResult status = glfwCreateWindowSurface(context_->GetInstance(), window_.get(), nullptr, &surface);
        VK_THROW_IF_FAILED(status, "Failed to create window surface!");
        swapchain_ = context_->CreateSwapchain(device_, surface, settings.width, settings.height);

        glfwMakeContextCurrent(window_.get());
    }

    void Application::Run()
    {
        while (!glfwWindowShouldClose(window_.get()))
        {
            // Main loop
            glfwSwapBuffers(window_.get());
            glfwPollEvents();
        }
    }

    Application::~Application()
    {
        glfwTerminate();
    }

}