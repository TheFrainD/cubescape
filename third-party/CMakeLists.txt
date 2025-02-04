# GLFW
set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
add_subdirectory(glfw EXCLUDE_FROM_ALL)

# glad
add_subdirectory(glad)

# cglm
add_subdirectory(cglm)

# stb_image
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/stb)

# FastNoiseLite
add_library(FastNoiseLite INTERFACE)
target_include_directories(FastNoiseLite INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/FastNoiseLite/C)

# cJSON
add_library(cjson STATIC cJSON/cJSON.c cJSON/cJSON.h)
target_include_directories(cjson PUBLIC cJSON)
