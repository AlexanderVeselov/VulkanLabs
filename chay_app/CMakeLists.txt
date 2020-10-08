set(SOURCES
    application.cpp
    application.hpp
    main.cpp
)

add_executable(ChayApp ${SOURCES})
target_include_directories(ChayApp PRIVATE .)
target_link_libraries(ChayApp PRIVATE ChayRender glfw3::glfw3 GpuApi)
set_target_properties(ChayApp
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${ChayEngine_SOURCE_DIR}/chay_app)
