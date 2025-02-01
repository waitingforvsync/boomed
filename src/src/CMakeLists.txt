if(MSVC)
	add_compile_options(/W4 /WX)
else()
	add_compile_options(-Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function)
endif()

add_executable("boomed" WIN32)
#set_target_properties("boomed" PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
target_include_directories("boomed" PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory("app")
add_subdirectory("base")
add_subdirectory("math")
add_subdirectory("platform_sdl3")
add_subdirectory("templates")
add_subdirectory("ui")
add_subdirectory("world")


#add_subdirectory("boomed")
#add_subdirectory("boomed_lib")
