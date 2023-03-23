cmake_minimum_required(VERSION 3.10)
project(lvgl_sim)
set(CMAKE_C_STANDARD 11)#C11
set(CMAKE_CXX_STANDARD 17)#C17
set(CMAKE_CXX_STANDARD_REQUIRED ON)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})
file(GLOB_RECURSE INCLUDES "./*.h" )
file(GLOB_RECURSE SOURCES "./Utils/*.c" )

SET(SDL2_DIR ${PROJECT_SOURCE_DIR}/SDL2/cmake)
find_package(SDL2 REQUIRED SDL2)
include_directories(${SDL2_INCLUDE_DIRS})

add_subdirectory(lvgl)
add_compile_definitions(LV_CONF_INCLUDE_SIMPLE)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) 
SET(CMAKE_CXX_FLAGS "-O3")

add_executable(${PROJECT_NAME} main.c mouse_cursor_icon.c ${SOURCES} ${INCLUDES})
target_link_libraries(${PROJECT_NAME} PRIVATE lvgl lvgl::examples lvgl::demos ${SDL2_BINDIR}/SDL2.dll)
add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E
        copy
        ${SDL2_BINDIR}/SDL2.dll
        ${EXECUTABLE_OUTPUT_PATH}/SDL2.dll
)
