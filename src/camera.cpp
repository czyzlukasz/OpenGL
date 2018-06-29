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

void Camera::SetMVP(unsigned int ID, float aspect_ratio)
{        
    int vertexColorLocation = glGetUniformLocation(ID, "ourColor");
    SetProjection(aspect_ratio);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    view = glm::lookAt(CamPos, CamPos + CamDir, CamUp);
    glUniform4f(vertexColorLocation, 0, 0, 0, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glUniformMatrix4fv(glGetUniformLocation(ID, "projection"), 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, &model[0][0]); 
    glUniformMatrix4fv(glGetUniformLocation(ID, "view"), 1, GL_FALSE, &view[0][0]);
}