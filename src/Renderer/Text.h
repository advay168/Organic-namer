#pragma once
#include "Common.h"

class TextRenderer
{
public:
  static void Text(const std::string& text,
                   const glm::vec2& pos,
                   float scale,
                   const glm::vec3& color);

  static void Text(const std::string& text, const glm::vec2& pos, float scale);

  static void CenteredText(const std::string& text,
                           const glm::vec2& pos,
                           float scale,
                           const glm::vec3& color);

  static void CenteredText(const std::string& text,
                           const glm::vec2& pos,
                           float scale);

protected:
  static void Init();

  static void Begin();

  static void End();
};
