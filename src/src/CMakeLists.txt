add_executable("boomed" WIN32)

if(MSVC)
	target_compile_options("boomed" PRIVATE /W4 /WX /wd4100 /wd4820 /wd4189)
else()
	target_compile_options("boomed" PRIVATE -Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function)
endif()


#set_target_properties("boomed" PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
target_include_directories("boomed" PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory("app")
add_subdirectory("base")
add_subdirectory("math")
add_subdirectory("platform_sdl3")
add_subdirectory("templates")
add_subdirectory("ui")
add_subdirectory("world")
