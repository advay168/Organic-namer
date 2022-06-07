#pragma once

#include <glm/glm.hpp>

enum class Direction
{
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
private:
  glm::vec3 m_position;
  glm::vec3 m_front;
  glm::vec3 m_up;
  glm::vec3 m_right;
  glm::vec3 m_worldUp;
  // euler Angles
  float m_yaw;
  float m_pitch;
  // camera options
  float m_movementSpeed;
  float m_mouseSensitivity;

  void UpdateCameraVectors();

public:
  float m_zoom;
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
         float yaw = YAW,
         float pitch = PITCH);
  Camera(float posX,
         float posY,
         float posZ,
         float upX,
         float upY,
         float upZ,
         float yaw,
         float pitch);
  glm::mat4 ComputeViewMatrix();

  void ProcessKeyboard(Direction direction, float deltaTime);
  void ProcessMouseMovement(float xoffset,
                            float yoffset,
                            bool constrainPitch = true);

  void ProcessMouseScroll(float yoffset);
};