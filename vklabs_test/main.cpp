#include "gtest/gtest.h"
#include "vk_context.hpp"
#include <memory>
#include <vector>

class VkTest : public ::testing::Test
{};

TEST_F(VkTest, ContextCreation)
{
    AppSettings settings;
    std::vector<char const*> required_extensions;

    ASSERT_NO_THROW(vklabs::VkContext(settings, required_extensions));
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
