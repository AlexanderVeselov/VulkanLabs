#include "application.hpp"
#include "videoapi/vulkan_command_buffer.hpp"
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
        std::shared_ptr<VulkanShader> vertex_shader = device_->CreateVertexShader("../vklabs/shaders/shader.vert.spv");
        std::shared_ptr<VulkanShader> pixel_shader = device_->CreatePixelShader("../vklabs/shaders/shader.frag.spv");

        std::size_t swapchain_images_count = swapchain_->GetImagesCount();
        pipelines_.resize(swapchain_images_count);
        cmd_buffers_.resize(swapchain_images_count);

        struct Vertex
        {
            float pos[3];
            float color[3];
        };

        const std::vector<Vertex> vertices =
        {
            {{ 0.0f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
            {{ 0.5f,  0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}
        };

        vertex_buffer_ = device_->CreateBuffer(vertices.size() * sizeof(Vertex), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
        vertex_buffer_->Write(vertices.data());

        for (std::size_t i = 0; i < swapchain_images_count; ++i)
        {
            pipelines_[i] = device_->CreateGraphicsPipeline(vertex_shader, pixel_shader, settings.width, settings.height, swapchain_->GetImage(i));

            cmd_buffers_[i] = device_->CreateGraphicsCommandBuffer();
            cmd_buffers_[i]->Begin();
            cmd_buffers_[i]->BeginGraphics(pipelines_[i]);
            cmd_buffers_[i]->BindVertexBuffer(vertex_buffer_);
            cmd_buffers_[i]->Draw(vertices.size());
            cmd_buffers_[i]->EndGraphics();
            cmd_buffers_[i]->End();
        }

        glfwMakeContextCurrent(window_.get());
    }

    void Application::Run()
    {
        while (!glfwWindowShouldClose(window_.get()))
        {
            // Main loop
            glfwSwapBuffers(window_.get());
            glfwPollEvents();

            device_->SubmitGraphicsCommandBuffer(cmd_buffers_[swapchain_->GetCurrentImageIndex()]);
            swapchain_->Present();
        }

        device_->GraphicsWaitIdle();

    }

    Application::~Application()
    {
        glfwTerminate();
    }

}
