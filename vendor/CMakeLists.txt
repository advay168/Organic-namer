project(vendor)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)

add_subdirectory(glfw)
add_subdirectory(freetype)

add_compile_definitions(IMGUI_USER_CONFIG="user_imgui_config.h")
file(GLOB THIRD_PARTY
                    glad/src/glad.c
                    stb_image.cpp
                    imgui/*
                    imgui/misc/freetype/imgui_freetype.cpp
                    imgui/backends/imgui_impl_glfw.cpp
                    imgui/backends/imgui_impl_opengl3.cpp)

add_library(${PROJECT_NAME} STATIC ${THIRD_PARTY})

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC 
                   ../src
                    glad/include/
                    glfw/include/
                    freetype/include/
                    glm/
                    stb/
                    imgui/
                    )
target_link_libraries(${PROJECT_NAME}
                    glfw
                    freetype
                    )
