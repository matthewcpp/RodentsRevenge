cmake_minimum_required(VERSION 3.11)

include (${CMAKE_CURRENT_SOURCE_DIR}/cmake/util_functions.cmake)

project (RodentsRevenge C)

set(game_src
	src/player.h src/player.c
	src/grid.h src/grid.c
	src/game.h src/game.c
	src/input.h
	src/point.h src/point.c
	src/enemy.h src/enemy.c
	src/entity.h src/entity.c
	src/spawner.h src/spawner.c
	src/animation.h
	src/util.h src/util.c
	src/clock.h src/clock.c
	src/vec2.c src/vec2.h
	src/pool.h src/pool.c)

set(drawing_src
	src/draw/renderer.h
	src/draw/spritesheet.h src/draw/spritesheet.c)

set(ui_src
	src/ui/clock.h src/ui/clock.c
	src/ui/ui.h src/ui/ui.c
	src/ui/menu.h src/ui/menu.c
	src/ui/score.h src/ui/score.c)

set(sdl_src
	src/sdl/sdl_input.h src/sdl/sdl_input.c
	src/sdl/sdl_display.h src/sdl/sdl_display.c
	src/sdl/sdl_animation.c src/sdl/sdl_animation.h
	src/sdl/sdl_game.h src/sdl/sdl_game.c
	src/sdl/sdl_renderer.h src/sdl/sdl_renderer.c)

set(platform_src
	src/desktop/desktop_main.c)

add_executable(RodentsRevenge ${game_src} ${ui_src} ${drawing_src} ${sdl_src} ${platform_src})
set_source_files_properties(src/desktop/desktop_main.c PROPERTIES COMPILE_DEFINITIONS ASSET_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}/assets")
set_compiler_options(RodentsRevenge)
target_compile_definitions(RodentsRevenge PUBLIC RR_PLATFORM_DESKTOP)

SET(ENABLE_TESTING ON CACHE BOOL "Generate Test Projects")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/cutil)

if (MSVC)
	include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
	find_package(sdl2 REQUIRED)
	find_package(sdl2_image REQUIRED)
	find_package(sdl2_ttf REQUIRED)

	target_link_libraries(RodentsRevenge sdl2::sdl2 sdl2_image::sdl2_image sdl2_ttf::sdl2_ttf cutil)
else()
	list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/sdl2-cmake)
	find_package(SDL2 REQUIRED)
	find_package(SDL2_image REQUIRED)
	find_package(SDL2_ttf REQUIRED)

	target_link_libraries(RodentsRevenge SDL2::Main SDL2::Image SDL2::TTF cutil)
	target_link_libraries(RodentsRevenge m)
endif()


