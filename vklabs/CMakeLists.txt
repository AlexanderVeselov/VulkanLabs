set(SOURCES
    app_settings.hpp
    vk_validation.hpp
    vk_shared_object.hpp
    vk_exception.hpp
    context.hpp
    vk_context.cpp
    vk_context.hpp
    device.hpp
    vk_device.cpp
    vk_device.hpp
    swapchain.hpp
    vk_swapchain.cpp
    vk_swapchain.hpp
)

add_library(VulkanLabs STATIC ${SOURCES})
target_include_directories(VulkanLabs PUBLIC .)
target_link_libraries(VulkanLabs Vulkan::Vulkan)
target_compile_features(VulkanLabs PUBLIC cxx_std_17)
