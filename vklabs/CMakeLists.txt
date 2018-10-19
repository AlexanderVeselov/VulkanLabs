set(VIDEOAPI_SOURCES
    videoapi/video_api.hpp
    videoapi/vulkan_api.hpp
    videoapi/vulkan_api.cpp
    videoapi/vulkan_validation.hpp
    videoapi/vulkan_shared_object.hpp
    videoapi/vulkan_exception.hpp
    videoapi/vulkan_device.hpp
    videoapi/vulkan_device.cpp
)

set(SOURCES
    ${VIDEOAPI_SOURCES}
    app_settings.hpp
    swapchain.hpp
    vk_swapchain.cpp
    vk_swapchain.hpp
    image_format.hpp
    pipeline.hpp
    vk_pipeline.hpp
    pipeline_manager.hpp
    vk_pipeline_manager.hpp
    vk_pipeline_manager.cpp
    shader.hpp
    vk_shader.hpp
    vk_shader.cpp
)

source_group("VideoAPI" FILES ${VIDEOAPI_SOURCES})

add_library(VulkanLabs STATIC ${SOURCES})
target_include_directories(VulkanLabs PUBLIC .)
target_link_libraries(VulkanLabs Vulkan::Vulkan)
target_compile_features(VulkanLabs PUBLIC cxx_std_17)
