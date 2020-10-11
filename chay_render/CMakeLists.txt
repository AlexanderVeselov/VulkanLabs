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
    scene.hpp
    scene.cpp
)

set(SHADER_SOURCES
    shaders/common.h
    shaders/shader.frag
    shaders/shader.vert
)

source_group("shaders" FILES ${SHADER_SOURCES})

add_library(ChayRender STATIC ${SOURCES} ${SHADER_SOURCES})
target_include_directories(ChayRender PUBLIC .)
target_include_directories(ChayRender PRIVATE ${ChayEngine_SOURCE_DIR}/dependencies/tinyobjloader)
target_link_libraries(ChayRender GpuApi)
target_compile_features(ChayRender PUBLIC cxx_std_17)
