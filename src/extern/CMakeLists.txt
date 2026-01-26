# SDL
set(SDL_STATIC OFF)
add_subdirectory("SDL-release-3.4.0")

# immer library
set(immer_BUILD_TESTS OFF)
set(immer_BUILD_EXAMPLES OFF)
set(immer_BUILD_DOCS OFF)
set(immer_BUILD_EXTRAS OFF)
add_subdirectory("immer-0.9.1")
