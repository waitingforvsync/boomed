# SDL
set(SDL_STATIC OFF)
add_subdirectory("SDL-release-3.4.0")

# immer library (not used, for now)
#set(immer_BUILD_TESTS OFF)
#set(immer_BUILD_EXAMPLES OFF)
#set(immer_BUILD_DOCS OFF)
#set(immer_BUILD_EXTRAS OFF)
#add_subdirectory("immer-0.9.1")

# Catch2
add_subdirectory("Catch2-3.12.0")

# GLAD
add_subdirectory("glad-2.0.8")

# GLFW
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory("glfw-3.4")
