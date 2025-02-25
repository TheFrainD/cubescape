set(TARGET core)
add_library_with_sources(${TARGET})

find_package(spdlog REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLAD REQUIRED)
find_package(glm REQUIRED)
find_package(EnTT REQUIRED)

target_link_libraries(${TARGET} PUBLIC glfw glad::glad spdlog::spdlog glm::glm EnTT::EnTT)
target_compile_definitions(${TARGET} PRIVATE GLFW_INCLUDE_NONE)