cmake_minimum_required (VERSION 3.9)
project (asteroids)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Includes & Libs
include_directories(/opt/homebrew/include)
link_directories(/opt/homebrew/lib)

add_compile_options(-g -Wall -Wextra -Wpedantic -Wl,--stack,16777216)

add_executable(main_game game.cc math.cc matrix.cc geometry.cc sdl2_renderer.cc opengl_renderer.cc sound.cc main_game.cc physics.cc sdl2_game_controller.cc timer.cc wavefront.cc)

target_link_libraries(main_game SDL2 SDL2_mixer "-framework OpenGL" GLEW)


# -----------------------
# GoogleTest Setup
# -----------------------
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/googletest googletest_binary_dir)

include(GoogleTest)  # optional, falls du gtest_discover_tests etc. nutzen willst

# Gemeinsames GTest-Include-Verzeichnis
set(GTEST_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/include)

# Funktion zum Erstellen von Test-Executables
function(add_unit_test name)
    add_executable(${name} ${ARGN})
    target_include_directories(${name} PRIVATE ${GTEST_INCLUDE_DIR})
    target_link_libraries(${name} gtest gtest_main)
endfunction()

# Tests
add_unit_test(math_test       math_test.cc       math.cc)
add_unit_test(matrix_test     matrix_test.cc     matrix.cc math.cc)
add_unit_test(geometry_test   geometry_test.cc   geometry.cc math.cc)
add_unit_test(physics_test    physics_test.cc    physics.cc geometry.cc math.cc timer.cc)
target_link_libraries(physics_test SDL2)  # SDL2 nötig für physics_test

add_unit_test(game_test       game_test.cc       game.cc physics.cc geometry.cc math.cc timer.cc)
target_link_libraries(game_test SDL2)

# Tests aktivieren
enable_testing()