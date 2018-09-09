project(VulkanLabs CXX)

cmake_minimum_required(VERSION 3.8)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

list(APPEND CMAKE_MODULE_PATH "${VulkanLabs_SOURCE_DIR}/cmake")

find_package(Vulkan)
find_package(glfw3)

add_subdirectory(dependencies/googletest)
add_subdirectory(vklabs)
add_subdirectory(vklabs_exe)
add_subdirectory(vklabs_test)
