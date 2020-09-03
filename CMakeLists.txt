project(VulkanLabs CXX)

cmake_minimum_required(VERSION 3.8)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

list(APPEND CMAKE_MODULE_PATH "${VulkanLabs_SOURCE_DIR}/cmake")

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

add_subdirectory(dependencies/googletest)
add_subdirectory(gpuapi)
add_subdirectory(vklabs)
add_subdirectory(vklabs_exe)
add_subdirectory(vklabs_test)
add_subdirectory(dependencies/SPIRV-Cross)

set(SPIRV_CROSS_PROJECTS
    spirv-cross
    spirv-cross-core
    spirv-cross-cpp
    spirv-cross-glsl
    spirv-cross-hlsl
    spirv-cross-msl
    spirv-cross-reflect
    spirv-cross-util
)

set_property(TARGET ${SPIRV_CROSS_PROJECTS} PROPERTY FOLDER "SPIRV-Cross")
