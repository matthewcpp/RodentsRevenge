cmake_minimum_required(VERSION 3.11)

include (${CMAKE_CURRENT_SOURCE_DIR}/cmake/util_functions.cmake)

project (RodentsRevenge C)

set(game_src
	src/debug.h src/debug.c
	src/player.h src/player.c
	src/grid.h src/grid.c
	src/game.h src/game_private.h src/game.c
	src/high_scores.h src/high_scores.c
	src/input.h
	src/point.h src/point.c
	src/enemy.h src/enemy.c
	src/entity.h src/entity.c
	src/spawner.h src/spawner.c
	src/yarn.h src/yarn.c
	src/util.h src/util.c
	src/clock.h src/clock.c
	src/vec2.c src/vec2.h
	src/pool.h src/pool.c)

set(drawing_src
	src/draw/color.h src/draw/color.c
	src/draw/rect.h src/draw/rect.c
	src/draw/renderer.h
	src/draw/sprite.h
	src/draw/spritesheet.h src/draw/spritesheet.c
	src/draw/animation.h src/draw/animation.c)

set(ui_src
	src/ui/basic_rect.h src/ui/basic_rect.c
	src/ui/common.h src/ui/common.c
	src/ui/element.h src/ui/element.c
	src/ui/basic_sprite.h src/ui/basic_sprite.c
	src/ui/clock.h src/ui/clock.c
	src/ui/game_ui.h src/ui/game_ui.c
	src/ui/game_over.h src/ui/game_over.c
	src/ui/high_scores_ui.h src/ui/high_scores_ui.c
	src/ui/title_ui.h src/ui/title_ui.c
	src/ui/button.h src/ui/button.c
	src/ui/score.h src/ui/score.c
	src/ui/scoreboard.h src/ui/scoreboard.c
	src/ui/lives.h src/ui/lives.c
	src/ui/level_select_dialog.h src/ui/level_select_dialog.c
	src/ui/text_input.h src/ui/text_input.c
	src/ui/onscreen_keyboard.h src/ui/onscreen_keyboard.c
	src/ui/pause_dialog.h src/ui/pause_dialog.c
	src/ui/high_score_dialog.h src/ui/high_score_dialog.c)

set(sdl_src
	src/sdl/sdl_input.h src/sdl/sdl_input.c
	src/sdl/sdl_display.h src/sdl/sdl_display.c
	src/sdl/sdl_game.h src/sdl/sdl_game.c
	src/sdl/sdl_renderer.h src/sdl/sdl_renderer.c)

set(platform_src
	src/desktop/desktop_main.c)

add_executable(RodentsRevenge ${game_src} ${ui_src} ${drawing_src} ${sdl_src} ${platform_src})

set(data_dir ${CMAKE_CURRENT_BINARY_DIR}/data)
if (NOT IS_DIRECTORY ${data_dir})
	file(MAKE_DIRECTORY ${data_dir})
endif()

set_property(
	SOURCE src/desktop/desktop_main.c
	APPEND
	PROPERTY COMPILE_DEFINITIONS
	ASSET_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}/assets" COMPILE_DEFINITIONS DATA_DIRECTORY="${data_dir}")

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


