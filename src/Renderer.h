#pragma once
#include "Common.h"

class Renderer
{
public:
  static void Init();
  static void Text(const std::string& text,
                   float x,
                   float y,
                   float scale,
                   const glm::vec3& color);

  static void Text(const std::string& text, float x, float y, float scale);

  static void Circle(const glm::vec2& center, float radius);

  static void Circle(const glm::vec2& center,
                     float radius,
                     const glm::vec3& color);

  static void BorderCircle(const glm::vec2& center,
                           float innerRadius,
                           const glm::vec3& innerColor,
                           float borderThickness,
                           const glm::vec3& borderColor);

  static void Begin();

  static void End();

private:
  static void CircleInit();
  static void TextInit();

  static void CircleClear();
  static void TextClear();

  static void CircleDraw();
  static void TextDraw();

  static GLFWwindow* window;
};