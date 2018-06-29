#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "glad.h"
#include "glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


class Camera
{
    public:
    glm::vec3 CamPos, CamTarget, CamDir, CamUp;

    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection; 

    float lastX, lastY;
    float pitch = 0, yaw = 0;
    float sensitivity = 0.02f;
    
    void SetProjection(float, float FOV = 45.0f, float zNear = 0.1f, float zFar = 5000.0f);
    void SetViev(void);
    void SetLastXY(float, float);
    void SetMVP(unsigned int, float);

    Camera(void)
    {
        CamPos = glm::vec3(10.0f, 0.0f, 10.0f);
        CamTarget = glm::vec3(10.0f, 0.0f, 11.0f);
        CamDir = glm::vec3(0.0f, 0.0f, 1.0f);
        CamUp = glm::vec3(0.0f, 1.0f, 0.0f);
    }
};


#endif