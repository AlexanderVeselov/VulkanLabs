#include "gtest/gtest.h"
//#include "videoapi/vk_context.hpp"
#include "entity_manager.hpp"
#include "component_manager.hpp"
#include "transform.hpp"
#include "entity.hpp"
#include <memory>
#include <vector>
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
*/
class EntityTest : public ::testing::Test
{};

TEST_F(EntityTest, EntityCreation)
{
    ComponentManager component_manager;
    EntityManager entity_manager(component_manager);
    auto entity = entity_manager.CreateEntity("some_entity");
    Transform transform;
    transform.Ping();
    entity->Ping();
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
