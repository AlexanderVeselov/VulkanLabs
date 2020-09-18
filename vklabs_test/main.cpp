#include "gtest/gtest.h"
//#include "videoapi/vk_context.hpp"
//#include "entity_manager.hpp"
//#include "component_manager.hpp"
//#include "transform.hpp"
//#include "entity.hpp"
#include <memory>
#include <vector>

#include "gpu_api.hpp"
#include "gpu_device.hpp"
#include "gpu_pipeline.hpp"
#include "gpu_queue.hpp"
#include "gpu_command_buffer.hpp"

/*
class VkTest : public ::testing::Test
{};

TEST_F(VkTest, ContextCreation)
{
    std::vector<char const*> required_extensions;

    // Create context
    std::shared_ptr<vklabs::Context> context;
    ASSERT_NO_THROW(context = std::make_shared<vklabs::VkContext>(required_extensions));
}

TEST_F(VkTest, DeviceCreation)
{
    std::vector<char const*> required_extensions;

    // Create context
    std::shared_ptr<vklabs::Context> context;
    ASSERT_NO_THROW(context = std::make_shared<vklabs::VkContext>(required_extensions));

    const std::vector<const char*> deviceExtensions =
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    // Create device
    std::shared_ptr<vklabs::Device> device;
    ASSERT_NO_THROW(device = context->CreateDevice(0, deviceExtensions));

}

class EntityTest : public ::testing::Test
{};

TEST_F(EntityTest, EntityCreation)
{
    ComponentManager component_manager;
    EntityManager entity_manager(component_manager);
    auto entity = entity_manager.CreateEntity("some_entity");
    entity->Ping();

    Transform* transform = component_manager.CreateComponent<Transform>(0);
    transform->Ping();
    std::cout << "Entity id: " << transform->GetEntityId() << "\n";

    ASSERT_TRUE(transform == component_manager.GetComponent<Transform>(transform->GetEntityId()));

}
*/

class GpuApiTest : public ::testing::Test
{};

TEST_F(GpuApiTest, PipelineCreation)
{
    auto gpu_api = gpu::Api::CreateD3D12Api();
    gpu::DevicePtr device = gpu_api->CreateDevice();
    auto pipeline = device->CreateGraphicsPipeline("shader.vs", "shader.ps");
    auto& gfx_queue = device->GetQueue(gpu::QueueType::kGraphics);
    auto cmd_buffer = gfx_queue.CreateCommandBuffer();
    cmd_buffer->BindGraphicsPipeline(pipeline);
    cmd_buffer->Draw(1, 0);

}

TEST_F(GpuApiTest, ImageCreation)
{
    auto gpu_api = gpu::Api::CreateD3D12Api();
    gpu::DevicePtr device = gpu_api->CreateDevice();
    gpu::ImagePtr image = device->CreateImage(256, 256);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
