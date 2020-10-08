#ifndef APPLICATION_HPP_
#define APPLICATION_HPP_

#include "app_settings.hpp"
#include "gpu_types.hpp"
#include "gpu_command_buffer.hpp"
#include "gpu_pipeline.hpp"
#include "gpu_sync.hpp"
#include "gpu_buffer.hpp"

#include <GLFW/glfw3.h>
#include <memory>
#include <vector>

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
        std::unique_ptr<gpu::Api> videoapi_;
        std::unique_ptr<gpu::Device> device_;
        std::unique_ptr<gpu::Swapchain> swapchain_;
        std::vector<gpu::GraphicsPipelinePtr> pipelines_;
        std::vector<gpu::CommandBufferPtr> cmd_buffers_;
        std::vector<gpu::FencePtr> fences_;
        gpu::BufferPtr vertex_buffer_;

    };

}

#endif // APPLICATION_HPP_
