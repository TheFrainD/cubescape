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

# stb_image
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/stb)

# FastNoiseLite
add_library(FastNoiseLite INTERFACE)
target_include_directories(FastNoiseLite INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/FastNoiseLite/C)

# entt
set(ENTT_NOEXCEPTION ON)
add_subdirectory(entt)
