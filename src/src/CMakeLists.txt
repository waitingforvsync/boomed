add_executable("boomed" WIN32)

target_include_directories("boomed" PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory("app")
add_subdirectory("math")
target_link_libraries("boomed" PRIVATE "SDL3::SDL3")
