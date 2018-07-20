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
#include "ioprocess.hpp"
#include "object.hpp"


using namespace std;

void processInputKey(GLFWwindow *window);
void processInputMouse(GLFWwindow* window, double xpos, double ypos);
void processInputScroll(GLFWwindow* window, double xpos, double ypos); 
void processInputMouseForwarder(void* context, GLFWwindow* window, double xpos, double ypos);


int main (void)
{
    //Initialise and set the window.
    Window::WindowInit();


    //Source code of vertex and fragment shaders. 
    Shader::ShaderInit("src/VertexShader", "src/FragmentShader");

    Buffer::BufferInit();
    Map map(32);


    //Set callback functions for processing mouse inputs.
    glfwSetCursorPosCallback(Window::ID, Iopcs::processInputMouse);
    glfwSetScrollCallback(Window::ID, Iopcs::processInputScroll); 

    int frames = 0;
    float currentTime = glfwGetTime();

    
    //Send data to GPU.
    Buffer::SetBufferData(map.Get_v_array_size() * sizeof(float), map.Get_vertices(),
                          map.Get_i_array_size() * sizeof(unsigned int), map.Get_indices());
    cout << "Number of triangles: " << map.Get_v_array_size() * sizeof(float) / 3 << "\n";

    glUseProgram(Shader::ID);
    glBindVertexArray(Buffer::VAO);
    Camera::SetProjection((float)(WINDOW_SIZE_X / WINDOW_SIZE_Y));

    /*
     *  Render loop keeps window refreshing as long
     *  as user decides to close it.
     */
    while(!glfwWindowShouldClose(Window::ID))
    {
              


        
        glm::mat4 model;

        //Set the background color.
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // float timeValue = glfwGetTime();

        // map.vertices[1] = glfwGetTime();

        Camera::view = glm::translate(Camera::view, glm::vec3(0.0f, 0.0f, -3.0f));


        glUniformMatrix4fv(glGetUniformLocation(Shader::ID, "projection"), 1, GL_FALSE, &Camera::projection[0][0]);


        Camera::view = glm::lookAt(Camera::CamPos, Camera::CamPos + Camera::CamDir, Camera::CamUp);

        int vertexColorLocation = glGetUniformLocation(Shader::ID, "ourColor");

        glUniform4f(vertexColorLocation, 0, 1, 0, 1.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUniformMatrix4fv(glGetUniformLocation(Shader::ID, "model"), 1, GL_FALSE, &model[0][0]); 
        glUniformMatrix4fv(glGetUniformLocation(Shader::ID, "view"), 1, GL_FALSE, &Camera::view[0][0]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffer::EBO);
        glDrawElements(GL_TRIANGLES, map.Get_i_array_size(), GL_UNSIGNED_INT, 0);


        glUniform4f(vertexColorLocation, 0, 0, 0, 1.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glUniformMatrix4fv(glGetUniformLocation(Shader::ID, "model"), 1, GL_FALSE, &model[0][0]); 
        glUniformMatrix4fv(glGetUniformLocation(Shader::ID, "view"), 1, GL_FALSE, &Camera::view[0][0]);
        // glDrawArrays(GL_TRIANGLES, 0, 200);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffer::EBO);
        glDrawElements(GL_TRIANGLES, map.Get_i_array_size(), GL_UNSIGNED_INT, 0);
        glClearDepth(1);


        ++frames;
        glfwSwapBuffers(Window::ID);
        glfwPollEvents();   
        Iopcs::processInputKey(Window::ID);
        glFlush();
        // glfwSwapBuffers(Window::ID);

    }


    cout << "Avg FPS: " << (float)frames /(glfwGetTime() - currentTime) << endl;
    cout << "Running time: " << glfwGetTime() << endl;
    /*
     *  Terminate the window to free resources.
     */
    glfwTerminate();
    return 0;
}
