set(SOURCES
    application.cpp
    application.hpp
    main.cpp
)

add_executable(VulkanLabs_exe ${SOURCES})
target_include_directories(VulkanLabs_exe PRIVATE .)
target_link_libraries(VulkanLabs_exe PRIVATE VulkanLabs)
#target_compile_features(VulkanLabs_exe PRIVATE cxx_std_17)
