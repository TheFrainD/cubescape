file(GLOB_RECURSE SOURCES src/*.c)
file(GLOB_RECURSE HEADERS include/*.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE include)

find_package(OpenGL REQUIRED)

target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE EXECUTABLE_NAME="${PROJECT_NAME}")

target_link_libraries(${PROJECT_NAME} glfw glad cglm stb cubelog cubegl FastNoiseLite)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Werror -Wno-missing-braces)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets)
