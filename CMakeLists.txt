cmake_minimum_required(VERSION 3.11)

include (${CMAKE_CURRENT_SOURCE_DIR}/cmake/util_functions.cmake)

project (RodentsRevenge C)

add_executable(RodentsRevenge src/desktop/desktop_main.c src/player.h src/player.c src/grid.h src/grid.c src/game.h src/game.c src/input.h src/sdl/sdl_input.h src/sdl/sdl_input.c src/sdl/sdl_display.h src/sdl/sdl_display.c src/point.h src/point.c src/enemy.h src/enemy.c src/entity.h src/entity.c src/spawn.h src/spawn.c src/animation.h src/sdl/sdl_animation.c src/sdl/sdl_animation.h src/sdl/sdl_game.h src/sdl/sdl_game.c src/util.h src/util.c src/clock.h src/clock.c)
set_source_files_properties(src/desktop/desktop_main.c PROPERTIES COMPILE_DEFINITIONS ASSET_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}/assets")
set_compiler_options(RodentsRevenge)
target_compile_definitions(RodentsRevenge PUBLIC RR_PLATFORM_DESKTOP)

if (UNIX AND NOT APPLE)
	list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
	find_package(SDL2 REQUIRED)
	find_package(SDL2_image REQUIRED)
	find_package(SDL2_ttf REQUIRED)

	target_link_libraries(RodentsRevenge SDL2::Main SDL2::Image SDL2::TTF)
else()
	include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
	find_package(sdl2 REQUIRED)
	find_package(sdl2_image REQUIRED)
	find_package(sdl2_ttf REQUIRED)

	target_link_libraries(RodentsRevenge sdl2::sdl2 sdl2_image::sdl2_image sdl2_ttf::sdl2_ttf)
endif()
