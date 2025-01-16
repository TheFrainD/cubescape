file(GLOB_RECURSE SOURCES src/*.c)
file(GLOB_RECURSE HEADERS include/*.h)

add_library(cubelog STATIC ${SOURCES} ${HEADERS})

target_include_directories(cubelog PUBLIC include)
target_include_directories(cubelog PRIVATE include/cubelog)
