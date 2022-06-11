cmake_minimum_required(VERSION 3.0)
set (CMAKE_CXX_STANDARD 17)
project(Organic-Namer)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)

if (NOT EMSCRIPTEN)
add_subdirectory(vendor/glfw)
endif()
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
                    # vendor/assimp/include/
                    # vendor/bullet/src/
                    vendor/glad/include/
                    vendor/glfw/include/
                    vendor/freetype/include/
                    vendor/glm/
                    vendor/stb/
                    )

file(GLOB THIRD_PARTY vendor/glad/src/glad.c vendor/stb_image.cpp)
file(GLOB_RECURSE SRC src/*)

add_compile_definitions(GLFW_INCLUDE_NONE)
add_executable(${PROJECT_NAME} ${SRC} ${THIRD_PARTY})

if(EMSCRIPTEN)
    set(CMAKE_CXX_FLAGS "-O2")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS " --preload-file res -s USE_GLFW=3 -s USE_WEBGL2=1 -s FULL_ES3=1 --shell-file ${CMAKE_SOURCE_DIR}/template.html")
endif()


if (NOT EMSCRIPTEN)
target_link_libraries(${PROJECT_NAME}
                    # assimp
                    glfw
                    freetype
                    # BulletDynamics BulletCollision 
                    # LinearMath
                    )
endif()
target_link_libraries(${PROJECT_NAME} freetype)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/Common.h)
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)
