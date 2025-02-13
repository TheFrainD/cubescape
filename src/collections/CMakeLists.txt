set(TARGET collections)
add_library_with_sources(${TARGET})
target_link_libraries(${TARGET} PUBLIC core)

add_subdirectory(test)