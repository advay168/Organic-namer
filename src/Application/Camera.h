#pragma once
#include "Common.h"

#include "InputState.h"

class Camera
{
public:
  Camera(InputState& inputState);

  void processInput(float deltaTime);

  void Home();

  void Forward(float deltaTime);
  void Backward(float deltaTime);
  void Right(float deltaTime);
  void Left(float deltaTime);

  void ZoomIn();
  void ZoomOut();

  void setCameraPos(const glm::vec3& newCameraPos);
  void setZoom(float newZoom);

  const glm::mat4& getViewMatrix();
  const glm::mat4& getInverseViewMatrix();
  const glm::mat4& getProjectionMatrix();

  void debugDisplay();

private:
  void recalculateMatrixes();

private:
  float zoom;
  glm::vec3 cameraPos;
  glm::mat4 view;
  glm::mat4 inverseView;
  glm::mat4 projection;

  InputState& inputState;
  static constexpr float movementSpeed = 500.0f;
  static constexpr float zoomSpeed = 0.3f;
};
