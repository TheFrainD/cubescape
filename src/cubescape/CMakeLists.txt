gather_sources(${PROJECT_NAME})
add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE include)

find_package(OpenGL REQUIRED)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(${PROJECT_NAME} PRIVATE ASSERTIONS_ENABLED)
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE EXECUTABLE_NAME="${PROJECT_NAME}")

target_link_libraries(${PROJECT_NAME} PRIVATE core)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wno-missing-braces)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
  INSTALL_RPATH "@executable_path"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets)

set(INSTALL_DIR "${CMAKE_SOURCE_DIR}/bin")

install(
  TARGETS ${PROJECT_NAME} core collections
  DESTINATION ${INSTALL_DIR}
)

install(
  DIRECTORY "${CMAKE_SOURCE_DIR}/assets"
  DESTINATION ${INSTALL_DIR}
)