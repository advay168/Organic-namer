#include "Camera.h"

Camera::Camera(InputState& inputState)
  : inputState(inputState)
{
  Home();
}

void Camera::processInput(float deltaTime)
{
  if (inputState.isKeyPressed(ImGuiKey_W)) {
    Forward(deltaTime);
  }
  if (inputState.isKeyPressed(ImGuiKey_A)) {
    Left(deltaTime);
  }
  if (inputState.isKeyPressed(ImGuiKey_S)) {
    Backward(deltaTime);
  }
  if (inputState.isKeyPressed(ImGuiKey_D)) {
    Right(deltaTime);
  }
}

void Camera::Home()
{
  zoom = 1.0f;
  cameraPos = { WIDTH / 2.0f, HEIGHT / 2.0f, 1.0f };
  recalculateMatrixes();
}

void Camera::Forward(float deltaTime)
{
  cameraPos.y += deltaTime * movementSpeed;
  recalculateMatrixes();
}

void Camera::Backward(float deltaTime)
{
  cameraPos.y -= deltaTime * movementSpeed;
  recalculateMatrixes();
}

void Camera::Right(float deltaTime)
{
  cameraPos.x += deltaTime * movementSpeed;
  recalculateMatrixes();
}

void Camera::Left(float deltaTime)
{
  cameraPos.x -= deltaTime * movementSpeed;
  recalculateMatrixes();
}

void Camera::ZoomIn()
{
  zoom += zoomSpeed;
  recalculateMatrixes();
}

void Camera::ZoomOut()
{
  zoom -= zoomSpeed;
  recalculateMatrixes();
}

void Camera::setCameraPos(const glm::vec3& newCameraPos)
{
  cameraPos = newCameraPos;
  recalculateMatrixes();
}

void Camera::setZoom(float newZoom)
{
  zoom = newZoom;
  recalculateMatrixes();
}

const glm::mat4& Camera::getViewMatrix()
{
  return view;
}

const glm::mat4& Camera::getInverseViewMatrix()
{
  return inverseView;
}

const glm::mat4& Camera::getProjectionMatrix()
{
  return projection;
}

void Camera::debugDisplay()
{
  DISPLAY(cameraPos);
  DISPLAY(zoom);
}

void Camera::recalculateMatrixes()
{
  glm::vec3 centre(glm::vec3(WIDTH / 2.0f, HEIGHT / 2.0f, 0.0f));
  view = glm::mat4(1.0f);
  view = glm::translate(view, centre);
  view = glm::scale(view, { zoom, zoom, 1.0f });
  view = glm::translate(view, -cameraPos);
  inverseView = glm::inverse(view);
  projection = glm::ortho(0.0f, (float)WIDTH, 0.0f, (float)HEIGHT);
}
