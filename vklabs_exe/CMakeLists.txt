set(SOURCES
    application.cpp
    application.hpp
    main.cpp
)

add_executable(VulkanLabs_exe ${SOURCES})
target_include_directories(VulkanLabs_exe PRIVATE .)
target_link_libraries(VulkanLabs_exe PRIVATE VulkanLabs glfw3::glfw3)
set_target_properties(VulkanLabs_exe
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${VulkanLabs_SOURCE_DIR}/vklabs_exe)

