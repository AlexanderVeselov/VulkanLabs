project(VulkanLabs CXX)

cmake_minimum_required(VERSION 3.8)

list(APPEND CMAKE_MODULE_PATH "${VulkanLabs_SOURCE_DIR}/cmake")

find_package(Vulkan)
find_package(glfw3)

set(SOURCES
    src/app_settings.hpp
    src/application.cpp
    src/application.hpp
    src/context.hpp
    src/vk_context.cpp
    src/vk_context.hpp
    src/main.cpp
)

add_executable(VulkanLabs ${SOURCES})
target_include_directories(VulkanLabs PUBLIC "${VulkanLabs_SOURCE_DIR}/src")
target_link_libraries(VulkanLabs Vulkan::Vulkan glfw3::glfw3)
target_compile_features(VulkanLabs PRIVATE cxx_std_17)
