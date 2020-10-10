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
    camera.hpp
    camera.cpp
)

add_library(ChayRender STATIC ${SOURCES})
target_include_directories(ChayRender PUBLIC .)
target_include_directories(ChayRender PRIVATE ${ChayEngine_SOURCE_DIR}/dependencies/tinyobjloader)
target_link_libraries(ChayRender GpuApi)
target_compile_features(ChayRender PUBLIC cxx_std_17)
