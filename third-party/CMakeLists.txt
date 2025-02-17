# GLFW
set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
add_subdirectory(glfw EXCLUDE_FROM_ALL)

# glad
add_subdirectory(glad)

# glm
add_subdirectory(glm)

# spdlog
set(SPDLOG_NO_EXCEPTIONS ON)
add_subdirectory(spdlog)

# cglm
add_library(cglm INTERFACE)
target_include_directories(cglm INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/cglm/include)

# stb_image
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/stb)

# FastNoiseLite
add_library(FastNoiseLite INTERFACE)
target_include_directories(FastNoiseLite INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/FastNoiseLite/C)

# cJSON
add_library(cjson STATIC cJSON/cJSON.c cJSON/cJSON.h)
target_include_directories(cjson PUBLIC cJSON)

# Unity
add_library(unity STATIC unity/src/unity.c unity/src/unity.h)
target_include_directories(unity PUBLIC unity/src)