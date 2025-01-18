target_sources("boomed_lib"
    PRIVATE
    "world/contour.c"
    "world/edge.c"
    "world/serialise.c"
    "world/vertex.c"
    "world/world.c"
    "world/zone.c"
    "arena.c"
    "bitset.c"
    "boomed.c"
    "json.c"
    "ops.c"
    "str.c"
)

target_include_directories("boomed_lib" PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
