cmake_minimum_required(VERSION 3.0)
set (CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Organic-Namer)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)

add_subdirectory(vendor/glfw)
add_subdirectory(vendor/freetype)

# option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
# option(ASSIMP_BUILD_SAMPLES OFF)
# option(ASSIMP_BUILD_TESTS OFF)
# add_subdirectory(vendor/assimp)

# option(BUILD_BULLET2_DEMOS OFF)
# option(BUILD_CPU_DEMOS OFF)
# option(BUILD_EXTRAS OFF)
# option(BUILD_OPENGL3_DEMOS OFF)
# option(BUILD_UNIT_TESTS OFF)
# add_subdirectory(vendor/bullet)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

endif()
include_directories(
                    src
                    vendor/glad/include/
                    vendor/glfw/include/
                    vendor/freetype/include/
                    vendor/glm/
                    vendor/stb/
                    vendor/imgui/
                    )

file(GLOB THIRD_PARTY 
                    vendor/glad/src/glad.c 
                    vendor/stb_image.cpp 
                    vendor/imgui/* 
                    vendor/imgui/backends/imgui_impl_glfw.cpp 
                    vendor/imgui/backends/imgui_impl_opengl3.cpp)
file(GLOB_RECURSE SRC src/*)

add_compile_definitions(GLFW_INCLUDE_NONE)
add_executable(${PROJECT_NAME} ${SRC} ${THIRD_PARTY})

target_link_libraries(${PROJECT_NAME}
                    glfw
                    freetype
                    )
target_link_libraries(${PROJECT_NAME} freetype)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/Common.h)
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)
