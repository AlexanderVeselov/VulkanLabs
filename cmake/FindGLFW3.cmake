include (FindPackageHandleStandardArgs)

set(glfw3_DIR "${ChayEngine_SOURCE_DIR}/dependencies/glfw")

find_library(glfw3_LIBRARY NAMES glfw glfw3 PATHS "${glfw3_DIR}/lib/x64")
find_path(glfw3_INCLUDE_DIR NAMES GLFW/glfw3.h PATHS "${glfw3_DIR}/include")

find_package_handle_standard_args(glfw3 DEFAULT_MSG glfw3_LIBRARY glfw3_INCLUDE_DIR)

add_library(glfw3::glfw3 UNKNOWN IMPORTED)
set_target_properties(glfw3::glfw3 PROPERTIES IMPORTED_LOCATION "${glfw3_LIBRARY}")
set_target_properties(glfw3::glfw3 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${glfw3_INCLUDE_DIR}")
