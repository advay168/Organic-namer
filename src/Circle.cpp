#include "Circle.h"

static std::string vertPath("res/shaders/cicle.vert");
static std::string fragPath("res/shaders/cicle.frag");

static std::vector<CircleData> circles;

Circle::Circle()
  : m_data({ {}, 0.0f })
{
  circles.push_back({ {}, 0.0f });
}

Circle::Circle(const glm::vec2& center, float radius)
{
  circles.push_back({ {}, 0.0f });
}
