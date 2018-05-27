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
