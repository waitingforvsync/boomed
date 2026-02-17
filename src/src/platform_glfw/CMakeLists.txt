target_include_directories("boomed" PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
find_package("OpenGL" REQUIRED)
target_link_libraries("boomed" PRIVATE "glad" "OpenGL::GL" "glfw")
add_subdirectory("platform")
