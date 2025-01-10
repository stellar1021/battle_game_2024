cmake_minimum_required(VERSION 3.22)

project(battle_game)
set(CMAKE_CXX_STANDARD 17)

if (MSVC)
    add_compile_options(/utf-8)
endif()

set(BATTLE_GAME_ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets)

# 查找 ZLIB 库
find_package(ZLIB REQUIRED)

# 查找 spdlog 库
find_package(spdlog CONFIG REQUIRED)

# 查找 stb 库
find_path(STB_INCLUDE_DIRS "stb_image.h")
if (NOT STB_INCLUDE_DIRS)
    message(FATAL_ERROR "Could not find stb_image.h")
endif()

add_subdirectory(external/grassland)
find_package(absl CONFIG REQUIRED)

list(APPEND BATTLE_GAME_EXTERNAL_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/external/abseil-cpp)
list(APPEND BATTLE_GAME_EXTERNAL_INCLUDE_DIRS ${GRASSLAND_INCLUDE_DIRS})

add_subdirectory(src)

target_compile_definitions(battle_game PRIVATE BATTLE_GAME_ASSETS_DIR="${BATTLE_GAME_ASSETS_DIR}/")
