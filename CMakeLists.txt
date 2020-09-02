cmake_minimum_required(VERSION 3.11)

include (${CMAKE_CURRENT_SOURCE_DIR}/cmake/util_functions.cmake)

project (RodentsRevenge C)

add_executable(RodentsRevenge main.c player.h player.c grid.h grid.c game.h game.c sdl_controller.h sdl_controller.c sdl_display.h sdl_display.c point.h point.c enemy.h enemy.c entity.h entity.c)
set_compiler_options(RodentsRevenge)

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
find_package(sdl2 REQUIRED)
find_package(sdl2_image REQUIRED)
find_package(sdl2_ttf REQUIRED)

target_link_libraries(RodentsRevenge sdl2::sdl2 sdl2_image::sdl2_image sdl2_ttf::sdl2_ttf)