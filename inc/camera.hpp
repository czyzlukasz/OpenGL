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
    static glm::vec3 CamPos, CamTarget, CamDir, CamUp;

    static glm::mat4 view;
    static glm::mat4 projection; 

    static float lastX, lastY;
    static float pitch, yaw;
    static float sensitivity;
    static float movementSpeed;
    
    static void SetProjection(float, float FOV = 45.0f, float zNear = 0.1f, float zFar = 5000.0f);
    void SetViev(void);
    void SetLastXY(float, float);

};



#endif