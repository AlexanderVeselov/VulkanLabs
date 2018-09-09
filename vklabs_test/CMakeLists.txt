project(VulkanLabs_test)
                            
add_executable(VulkanLabs_test main.cpp)
target_link_libraries(VulkanLabs_test PUBLIC VulkanLabs GoogleTest)
