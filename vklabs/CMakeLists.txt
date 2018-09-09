set(SOURCES
    app_settings.hpp
    context.hpp
    vk_context.cpp
    vk_context.hpp
    vk_validation.hpp
    device.hpp
    vk_device.cpp
    vk_device.hpp
    vk_shared_object.hpp
)

add_library(VulkanLabs STATIC ${SOURCES})
target_include_directories(VulkanLabs PUBLIC .)
target_link_libraries(VulkanLabs Vulkan::Vulkan)
target_compile_features(VulkanLabs PUBLIC cxx_std_17)
