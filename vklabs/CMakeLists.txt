set(VIDEOAPI_SOURCES
    videoapi/video_api.hpp
    videoapi/vulkan_api.hpp
    videoapi/vulkan_api.cpp
    videoapi/vulkan_validation.hpp
    videoapi/vulkan_shared_object.hpp
    videoapi/vulkan_exception.hpp
    videoapi/vulkan_device.hpp
    videoapi/vulkan_device.cpp
    videoapi/vulkan_swapchain.hpp
    videoapi/vulkan_swapchain.cpp
    videoapi/vulkan_shader.hpp
    videoapi/vulkan_shader.cpp
    videoapi/vulkan_graphics_pipeline.hpp
    videoapi/vulkan_graphics_pipeline.cpp
    videoapi/vulkan_command_buffer.hpp
    videoapi/vulkan_command_buffer.cpp
    videoapi/vulkan_image.hpp
    videoapi/vulkan_image.cpp
    videoapi/vulkan_buffer.hpp
    videoapi/vulkan_buffer.cpp
    videoapi/vulkan_memory_manager.hpp
    videoapi/vulkan_memory_manager.cpp
)

set(SOURCES
    ${VIDEOAPI_SOURCES}
    app_settings.hpp
)

source_group("VideoAPI" FILES ${VIDEOAPI_SOURCES})

add_library(VulkanLabs STATIC ${SOURCES})
target_include_directories(VulkanLabs PUBLIC .)
target_link_libraries(VulkanLabs Vulkan::Vulkan)
target_compile_features(VulkanLabs PUBLIC cxx_std_17)
