target_sources("boomed_lib"
    PRIVATE
    "arena.c"
    "world.c"
)

target_include_directories("boomed_lib" PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
