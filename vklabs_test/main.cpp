#include "gtest/gtest.h"
#include "vk_context.hpp"
#include <memory>
#include <vector>

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

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
