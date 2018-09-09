project(GoogleTest)

set(SOURCES
    src/gtest-all.cc
    src/gtest_main.cc)

add_library(GoogleTest STATIC ${SOURCES})

target_include_directories(GoogleTest PUBLIC include)
target_include_directories(GoogleTest PRIVATE .)
target_compile_definitions(GoogleTest PUBLIC _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING)
