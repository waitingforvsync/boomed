if(MSVC)
#	add_compile_options(/W4 /WX)
else()
#	add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

add_subdirectory("boomed")
add_subdirectory("boomed_lib")
