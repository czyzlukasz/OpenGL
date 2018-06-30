#include <iostream>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "camera.hpp"
#include "window.hpp"
#include "map.hpp"


using namespace std;

void processInputKey(GLFWwindow *window);
void processInputMouse(GLFWwindow* window, double xpos, double ypos);
void processInputScroll(GLFWwindow* window, double xpos, double ypos); 
void processInputMouseForwarder(void* context, GLFWwindow* window, double xpos, double ypos);

/*
 *  Window object holding all data of current window.
 */
// GLFWwindow *window;

    Camera camera;


int main (void)
{

    //Initialise and set the window.
    Window window;

    //Source code of vertex and fragment shaders. 
    Shader shader("src/VertexShader", "src/FragmentShader");
    Buffer buffer;
    Map map(1024);


    //Set callback functions for processing mouse inputs.
    glfwSetCursorPosCallback(window.ID, processInputMouse);
    glfwSetScrollCallback(window.ID, processInputScroll); 

    int frames = 0;
    float currentTime = glfwGetTime();

    //Send data to GPU.
    buffer.SetBufferData(map.Get_v_array_size() * sizeof(float), map.Get_vertices(),
                         map.Get_i_array_size() * sizeof(unsigned int), map.Get_indices());
    cout << "Number of triangles: " << map.Get_v_array_size() * sizeof(float) / 3 << "\n";
    /*
     *  Render loop keeps window refreshing as long
     *  as user decides to close it.
     */
    // cout << map.i_array_size << "    " <<  map.v_array_size << endl;
    while(!glfwWindowShouldClose(window.ID))
    {
              

        glm::mat4 model;

        //Set the background color.
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // float timeValue = glfwGetTime();

        // map.vertices[1] = glfwGetTime();
        glUseProgram(shader.ID);
        glBindVertexArray(buffer.VAO);
        camera.SetProjection((float)(WINDOW_SIZE_X / WINDOW_SIZE_Y));
        camera.view = glm::translate(camera.view, glm::vec3(0.0f, 0.0f, -3.0f));


        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, &camera.projection[0][0]);


        camera.view = glm::lookAt(camera.CamPos, camera.CamPos + camera.CamDir, camera.CamUp);

        int vertexColorLocation = glGetUniformLocation(shader.ID, "ourColor");
        glUniform4f(vertexColorLocation, 0, 1, 0, 1.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, &model[0][0]); 
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, &camera.view[0][0]);
        // glDrawArrays(GL_TRIANGLES, 0, 100);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.EBO);
        glDrawElements(GL_TRIANGLES, map.Get_i_array_size(), GL_UNSIGNED_INT, 0);


        glUniform4f(vertexColorLocation, 0, 0, 0, 1.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, &model[0][0]); 
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, &camera.view[0][0]);
        // glDrawArrays(GL_TRIANGLES, 0, 200);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.EBO);
        glDrawElements(GL_TRIANGLES, map.Get_i_array_size(), GL_UNSIGNED_INT, 0);
        glClearDepth(1);


        ++frames;
        glfwSwapBuffers(window.ID);
        glfwPollEvents();   
        processInputKey(window.ID);
        glFlush();
        // glfwSwapBuffers(window.ID);

    }


    cout << "Avg FPS: " << (float)frames /(glfwGetTime() - currentTime) << endl;
    cout << "Running time: " << glfwGetTime() << endl;
    /*
     *  Terminate the window to free resources.
     */
    glfwTerminate();
    return 0;
}

/*
 *  Function interfacing program with keyboard.
 */

void processInputKey(GLFWwindow *window)
{

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {    
        camera.CamPos = glm::vec3(0.0f, 0.0f, 0.0f);
        camera.CamDir = glm::vec3(0.0f, 0.0f, -1.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.CamPos -= camera.CamDir * camera.sensitivity;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.CamPos += camera.CamDir * camera.sensitivity;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.CamPos -= glm::normalize(glm::cross(camera.CamDir, camera.CamUp)) * camera.sensitivity;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.CamPos += glm::normalize(glm::cross(camera.CamDir, camera.CamUp)) * camera.sensitivity;
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        camera.CamPos -= glm::normalize(glm::cross(glm::cross(camera.CamUp, camera.CamDir), camera.CamDir)) * camera.sensitivity;
    }

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        camera.CamPos += glm::normalize(glm::cross(glm::cross(camera.CamUp, camera.CamDir), camera.CamDir)) * camera.sensitivity;
    }
}

void processInputMouse(GLFWwindow* window, double xpos, double ypos)
{
    float dx = xpos - camera.lastX;
    float dy = camera.lastY - ypos;

    float sensitivity = 0.05f;
    dx *= sensitivity;
    dy *= sensitivity;

    camera.lastX = xpos;
    camera.lastY = ypos;
    
    
    camera.yaw   += dx;
    camera.pitch += dy;
    if(camera.pitch > 89.0f)
        camera.pitch =  89.0f;
    if(camera.pitch < -89.0f)
        camera.pitch = -89.0f;

    camera.CamDir.x = cos(glm::radians(camera.pitch)) * cos(glm::radians(camera.yaw));
    camera.CamDir.y = sin(glm::radians(camera.pitch));
    camera.CamDir.z = cos(glm::radians(camera.pitch)) * sin(glm::radians(camera.yaw));
    camera.CamDir = glm::normalize(camera.CamDir);
}

void processInputScroll(GLFWwindow* window, double xpos, double ypos)
{
    camera.sensitivity +=ypos/10;
    if(camera.sensitivity < 0) camera.sensitivity = 0.05;
    if(camera.sensitivity > 2) camera.sensitivity = 2;
}