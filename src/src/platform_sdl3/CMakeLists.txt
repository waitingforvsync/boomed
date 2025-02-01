target_sources("boomed"
    PRIVATE
    "main_sdl3.c"
    "main_sdl3.h"
)

target_link_libraries("boomed" PRIVATE "SDL3-static")
