#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>

class Text
{
public:
  static int init(GLFWwindow* window);
  static void render(const std::string& text,
                     float x,
                     float y,
                     float scale,
                     const glm::vec3& color = glm::vec3(1.0f));
};