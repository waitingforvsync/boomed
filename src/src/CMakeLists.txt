add_executable("boomed" WIN32)

target_include_directories("boomed" PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options("boomed" PRIVATE
    "$<$<CXX_COMPILER_ID:MSVC>:/W4;/WX;/wd4100;/wd4820;/wd4189;/wd4206;/wd4101>"
    "$<$<CXX_COMPILER_ID:Clang,GNU>:-Wall;-Wextra;-Wpedantic;-Werror;-Wno-unused-parameter;-Wno-unused-variable;-Wno-unused-function>"
)

add_subdirectory("app")
add_subdirectory("commands")
add_subdirectory("math")
add_subdirectory("platform_glfw")
add_subdirectory("utils")
add_subdirectory("world")
