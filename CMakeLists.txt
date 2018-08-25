project(VulkanLabs CXX)

cmake_minimum_required(VERSION 3.8)

set(SOURCES
    src/main.cpp
)

find_package(Vulkan)

add_executable(VulkanLabs ${SOURCES})
target_include_directories(VulkanLabs PUBLIC "${VulkanLabs_SOURCE_DIR}")
target_link_libraries(VulkanLabs Vulkan::Vulkan)
target_compile_features(VulkanLabs PRIVATE cxx_std_17)
