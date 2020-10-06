#include "application.hpp"

#include "gpu_api.hpp"
#include "gpu_device.hpp"
#include "gpu_swapchain.hpp"
#include "gpu_queue.hpp"

#include <stdexcept>
#include <iostream>
#include <vector>
#include <vulkan/vulkan.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

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
        /*
        std::uint32_t glfw_extension_count;
        char const** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
        std::vector<char const*> required_extensions(glfw_extensions, glfw_extensions + glfw_extension_count);
        */

        videoapi_.reset(gpu::Api::CreateD3D12Api());
        device_ = videoapi_->CreateDevice();

        HWND hwnd = glfwGetWin32Window(window_.get());

        swapchain_ = device_->CreateSwapchain(hwnd, settings.width, settings.height);

        auto& swapchain_images = swapchain_->GetImages();
        //pipelines_.resize(swapchain_images_count);
        //cmd_buffers_.resize(swapchain_images.size());

        auto& queue = device_->GetQueue(gpu::QueueType::kGraphics);
        for (auto i = 0; i < swapchain_images.size(); ++i)
        {
            auto& image = swapchain_images[i];

            gpu::GraphicsPipelineDesc pipeline_desc;
            pipeline_desc.vs_filename = "shader.vs";
            pipeline_desc.ps_filename = "shader.ps";
            pipeline_desc.color_attachments.push_back(image);
            auto pipeline = device_->CreateGraphicsPipeline(pipeline_desc);

            auto cmd_buffer = queue.CreateCommandBuffer();
            cmd_buffer->TransitionBarrier(image, gpu::ImageLayout::kPresent, gpu::ImageLayout::kRenderTarget);
            cmd_buffer->ClearImage(image, 0.5f, 0.5f, 1.0f, 1.0f);
            cmd_buffer->BindGraphicsPipeline(pipeline);
            cmd_buffer->Draw(3, 0);
            cmd_buffer->TransitionBarrier(image, gpu::ImageLayout::kRenderTarget, gpu::ImageLayout::kPresent);
            cmd_buffer->End();

            cmd_buffers_.push_back(std::move(cmd_buffer));
            pipelines_.push_back(std::move(pipeline));
            fences_.push_back(device_->CreateFence());
        }

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

        /*
        vertex_buffer_ = device_->CreateBuffer(vertices.size() * sizeof(Vertex), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
        vertex_buffer_->Write(vertices.data());

        float vs_data[3] = { 1.0f, 0.0f, 0.0f };
        vs_uniform_buffer_ = device_->CreateBuffer(sizeof(vs_data), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
        vs_uniform_buffer_->Write(&vs_data);

        float ps_data[3] = { 0.0f, 1.0f, 0.0f };
        ps_uniform_buffer_ = device_->CreateBuffer(sizeof(ps_data), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
        ps_uniform_buffer_->Write(&ps_data);

        for (std::size_t i = 0; i < swapchain_images_count; ++i)
        {
            VulkanGraphicsPipelineState pipeline_state(settings.width, settings.height);
            pipeline_state.SetVertexShader(vertex_shader);
            pipeline_state.SetPixelShader(pixel_shader);
            pipeline_state.SetColorAttachment(0, swapchain_->GetImage(i));

            pipelines_[i] = device_->CreateGraphicsPipeline(pipeline_state);
            pipelines_[i]->SetArg(0, 0, vs_uniform_buffer_);
            pipelines_[i]->SetArg(0, 1, ps_uniform_buffer_);
            pipelines_[i]->CommitArgs();

            cmd_buffers_[i] = device_->CreateGraphicsCommandBuffer();
            cmd_buffers_[i]->Begin();
            cmd_buffers_[i]->BeginGraphics(pipelines_[i]);
            cmd_buffers_[i]->BindVertexBuffer(vertex_buffer_);
            cmd_buffers_[i]->Draw(vertices.size());
            cmd_buffers_[i]->EndGraphics();
            cmd_buffers_[i]->End();
        }
        */

        glfwMakeContextCurrent(window_.get());
    }

    void Application::Run()
    {
        auto& queue = device_->GetQueue(gpu::QueueType::kGraphics);

        std::uint32_t frame_index = 0;
        while (!glfwWindowShouldClose(window_.get()))
        {
            // Main loop
            glfwSwapBuffers(window_.get());
            glfwPollEvents();

            fences_[frame_index]->Wait();
            queue.Submit(cmd_buffers_[frame_index], fences_[frame_index]);
            swapchain_->Present();
            frame_index = (frame_index + 1) % cmd_buffers_.size();

        }

    }

    Application::~Application()
    {
        glfwTerminate();
    }

}
