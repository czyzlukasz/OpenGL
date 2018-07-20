#ifndef IOPROCESS_HPP
#define IOPROCESS_HPP

class Iopcs{
    public:
    static void processInputScroll(GLFWwindow* window, double xpos, double ypos){
        Camera::movementSpeed +=ypos/10;
        if(Camera::movementSpeed < 0) Camera::movementSpeed = 0.05;
        if(Camera::movementSpeed > 2) Camera::movementSpeed = 2;
    }

    /*
     *  Function interfacing program with keyboard.
     */
    static void processInputKey(GLFWwindow *window)
    {

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        {    
            Camera::CamPos = glm::vec3(0.0f, 0.0f, 0.0f);
            Camera::CamDir = glm::vec3(0.0f, 0.0f, -1.0f);
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            Camera::CamPos -= Camera::CamDir * Camera::movementSpeed;
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            Camera::CamPos += Camera::CamDir * Camera::movementSpeed;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            Camera::CamPos -= glm::normalize(glm::cross(Camera::CamDir, Camera::CamUp)) * Camera::movementSpeed;
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            Camera::CamPos += glm::normalize(glm::cross(Camera::CamDir, Camera::CamUp)) * Camera::movementSpeed;
        }

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            Camera::CamPos -= glm::normalize(glm::cross(glm::cross(Camera::CamUp, Camera::CamDir), Camera::CamDir)) * Camera::movementSpeed;
        }

        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        {
            Camera::CamPos += glm::normalize(glm::cross(glm::cross(Camera::CamUp, Camera::CamDir), Camera::CamDir)) * Camera::movementSpeed;
        }
    }

    static void processInputMouse(GLFWwindow* window, double xpos, double ypos)
    {
        float dx = xpos - Camera::lastX;
        float dy = Camera::lastY - ypos;

        dx *= Camera::sensitivity;
        dy *= Camera::sensitivity;

        Camera::lastX = xpos;
        Camera::lastY = ypos;
        
        
        Camera::yaw   += dx;
        Camera::pitch += dy;
        if(Camera::pitch > 89.0f)
            Camera::pitch =  89.0f;
        if(Camera::pitch < -89.0f)
            Camera::pitch = -89.0f;

        Camera::CamDir.x = cos(glm::radians(Camera::pitch)) * cos(glm::radians(Camera::yaw));
        Camera::CamDir.y = sin(glm::radians(Camera::pitch));
        Camera::CamDir.z = cos(glm::radians(Camera::pitch)) * sin(glm::radians(Camera::yaw));
        Camera::CamDir = glm::normalize(Camera::CamDir);
    }
};

#endif