file(GLOB_RECURSE SOURCES src/*.c)
file(GLOB_RECURSE HEADERS include/*.h)

add_library(cubegl STATIC ${SOURCES} ${HEADERS})

target_include_directories(cubegl PUBLIC include)
target_include_directories(cubegl PRIVATE include/cubegl)

target_link_libraries(cubegl glad stb cubelog)
