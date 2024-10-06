cmake_minimum_required(VERSION 3.0)
set (CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Organic-Namer)
add_subdirectory(vendor)

file(GLOB_RECURSE SRC src/*)
add_executable(${PROJECT_NAME} ${SRC})

target_include_directories(${PROJECT_NAME} PRIVATE src)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
endif()


add_compile_definitions(IMGUI_USER_CONFIG="user_imgui_config.h")
add_compile_definitions(GLFW_INCLUDE_NONE)

target_link_libraries(${PROJECT_NAME} vendor)

target_precompile_headers(${PROJECT_NAME} PRIVATE src/Common.h)
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/imgui.ini $<TARGET_FILE_DIR:${PROJECT_NAME}>/imgui.ini)
