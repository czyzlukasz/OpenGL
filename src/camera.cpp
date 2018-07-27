#include <iostream>
#include "camera.hpp"


void Camera::SetProjection(float aspect_ratio, float FOV, float zNear, float zFar)
{
    projection = glm::perspective(glm::radians(FOV), aspect_ratio, zNear, zFar);
}

void Camera::SetViev(void)
{
    glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
}

void Camera::SetLastXY(float x, float y)
{
    lastX = x;
    lastY = y;
}
float Camera::sensitivity = 0.05f;
float Camera::movementSpeed = 0.25f;
glm::vec3 Camera::CamUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 Camera::CamPos = glm::vec3(-1.0f, 0.0f, 0.0f);
glm::vec3 Camera::CamTarget = glm::vec3(-1.0f, 0.0f, 1.0f);
glm::vec3 Camera::CamDir = glm::vec3(0.0f, 0.0f, 0.0f);
glm::mat4 Camera::view{};// = glm::mat4();
glm::mat4 Camera::projection{}; 
float Camera::lastX{};
float Camera::lastY{};
float Camera::pitch{};
float Camera::yaw{};