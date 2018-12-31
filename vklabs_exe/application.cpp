#include "application.hpp"
#include <stdexcept>
#include <iostream>
#include <vector>
#include <vulkan/vulkan.h>

namespace vklabs
{
    Application::Application(AppSettings const& settings)
        : settings_(settings)
        , window_(nullptr, glfwDestroyWindow)
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

        videoapi_.reset(new VulkanAPI(required_extensions, true));

        const std::vector<const char*> device_extensions =
        {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        GLFWwindow* glfw_window = window_.get();
        device_ = videoapi_->CreateDevice(device_extensions, 0, [glfw_window](VkInstance instance)
        {
            VkSurfaceKHR surface;
            VkResult status = glfwCreateWindowSurface(instance, glfw_window, nullptr, &surface);
            if (status != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to create window surface!");
            }

            return surface;
        });

        swapchain_ = device_->CreateSwapchain(settings.width, settings.height);
        std::shared_ptr<VulkanShader> vertex_shader = device_->CreateShader("../vklabs/shader.vert.spv");
        std::shared_ptr<VulkanShader> pixel_shader = device_->CreateShader("../vklabs/shader.frag.spv");

        device_->CreateGraphicsPipeline(vertex_shader, pixel_shader);

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