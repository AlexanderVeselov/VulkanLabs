add_executable(ChayTest main.cpp)
target_link_libraries(ChayTest PUBLIC GoogleTest GpuApi)
set_target_properties(ChayTest 
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${ChayEngine_SOURCE_DIR}/chay_test)
