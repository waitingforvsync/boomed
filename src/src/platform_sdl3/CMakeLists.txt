target_include_directories("boomed" PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries("boomed" PRIVATE "SDL3::SDL3")
add_subdirectory("platform")
