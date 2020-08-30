cmake_minimum_required(VERSION 3.11)

include (${CMAKE_CURRENT_SOURCE_DIR}/cmake/util_functions.cmake)

project (RodentsRevenge C)

add_executable(RodentsRevenge main.c player.h player.c grid.h grid.c game.h game.c sdl_controller.h sdl_controller.c sdl_renderer.h sdl_renderer.c)
set_compiler_options(RodentsRevenge)

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
find_package(SDL2 REQUIRED)

target_link_libraries(RodentsRevenge SDL2::SDL2)