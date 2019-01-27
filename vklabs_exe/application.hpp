#ifndef APPLICATION_HPP_
#define APPLICATION_HPP_

#include "app_settings.hpp"
#include "videoapi/vulkan_api.hpp"
#include "videoapi/vulkan_device.hpp"
#include "videoapi/vulkan_swapchain.hpp"
#include "videoapi/vulkan_buffer.hpp"
#include "videoapi/vulkan_graphics_pipeline.hpp"
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
        std::shared_ptr<VulkanAPI> videoapi_;
        std::shared_ptr<VulkanDevice> device_;
        std::shared_ptr<VulkanSwapchain> swapchain_;
        std::vector<std::shared_ptr<VulkanGraphicsPipeline>> pipelines_;
        std::vector<std::shared_ptr<VulkanCommandBuffer>> cmd_buffers_;
        std::shared_ptr<VulkanBuffer> vertex_buffer_;
        std::shared_ptr<VulkanBuffer> uniform_buffer_;

    };

}

#endif // APPLICATION_HPP_
