file(GLOB_RECURSE collections_test_SOURCES  "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
add_executable(collections_test ${collections_test_SOURCES})
target_link_libraries(collections_test PRIVATE collections unity)

enable_testing()