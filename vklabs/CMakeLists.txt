set(SOURCES
    app_settings.hpp
    entity.hpp
    entity.cpp
    entity_manager.hpp
    entity_manager.cpp
    component.hpp
    component.cpp
    component_manager.hpp
    component_manager.cpp
    transform.hpp
    transform.cpp
    renderable.hpp
    mesh.hpp
    mesh.cpp
)

add_library(VulkanLabs STATIC ${SOURCES})
target_include_directories(VulkanLabs PUBLIC .)
target_include_directories(VulkanLabs PRIVATE ${VulkanLabs_SOURCE_DIR}/dependencies/tinyobjloader)
target_link_libraries(VulkanLabs GpuApi)
target_compile_features(VulkanLabs PUBLIC cxx_std_17)
