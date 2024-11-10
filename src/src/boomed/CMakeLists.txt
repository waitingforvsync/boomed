add_executable("boomed" WIN32)
target_link_libraries("boomed" PRIVATE "boomed_lib")
target_link_libraries("boomed" PRIVATE "SDL3-static")
add_subdirectory("src")
