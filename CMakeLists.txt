cmake_minimum_required(VERSION 3.11)

include (${CMAKE_CURRENT_SOURCE_DIR}/cmake/util_functions.cmake)

project (RodentsRevenge C)

add_executable(RodentsRevenge main.c player.h player.c grid.h grid.c game.h game.c sdl_controller.h sdl_controller.c sdl_display.h sdl_display.c point.h point.c enemy.h enemy.c entity.h entity.c)
set_source_files_properties(main.c PROPERTIES COMPILE_DEFINITIONS ASSET_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}/assets")
set_compiler_options(RodentsRevenge)

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
