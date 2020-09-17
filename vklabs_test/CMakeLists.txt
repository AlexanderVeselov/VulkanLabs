project(VulkanLabs_test)
                            
add_executable(VulkanLabs_test main.cpp)
target_link_libraries(VulkanLabs_test PUBLIC GoogleTest GpuApi)
set_target_properties(VulkanLabs_test 
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${VulkanLabs_SOURCE_DIR}/vklabs_test)
