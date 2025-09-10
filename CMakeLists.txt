cmake_minimum_required(VERSION 3.10)
project(PhysicsEngine C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -g")

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_SOURCE_DIR}/include)

set(SRC_FILES
    ${SRC_DIR}/main.c
    ${SRC_DIR}/rect.c
    ${SRC_DIR}/utils.c
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)

add_executable(PhysicsEngine ${SRC_FILES})

target_include_directories(PhysicsEngine PRIVATE ${INC_DIR})
target_link_libraries(PhysicsEngine PRIVATE PkgConfig::SDL2)