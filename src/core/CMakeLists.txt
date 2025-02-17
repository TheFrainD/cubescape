set(TARGET core)
add_library_with_sources(${TARGET})
target_link_libraries(${TARGET} PUBLIC glfw glad spdlog::spdlog glm::glm EnTT::EnTT)
target_compile_definitions(${TARGET} PRIVATE GLFW_INCLUDE_NONE)