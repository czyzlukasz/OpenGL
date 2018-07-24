#include <iostream>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "camera.hpp"
#include "window.hpp"
// #include "map.hpp"
#include "ioprocess.hpp"
#include "model.hpp"


using namespace std;

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};


int main (void)
{
    //Initialise and set the window.
    Window::WindowInit();


    //Source code of vertex and fragment shaders. 
    Shader objShader("src/VertexShader", "src/FragmentShader");
    // Shader lightShader("src/LightShader", "src/FragmentShader");

    Buffer::BufferInit();
    // Map map(512);


    //Set callback functions for processing mouse inputs.
    glfwSetCursorPosCallback(Window::ID, Iopcs::processInputMouse);
    glfwSetScrollCallback(Window::ID, Iopcs::processInputScroll); 

    int frames = 0;
    float currentTime = glfwGetTime();

    
    // //Send data to GPU.
    // Buffer::SetBufferData(map.Get_v_array_size() * sizeof(float), map.Get_vertices(),
    //                       map.Get_i_array_size() * sizeof(unsigned int), map.Get_indices());
    // cout << "Number of triangles: " << map.Get_v_array_size() * sizeof(float) / 3 << "\n";

    Buffer::SetBufferData(sizeof(vertices), vertices);


    glUseProgram(objShader.ID);
    glBindVertexArray(Buffer::VAO);
    Camera::SetProjection((float)(WINDOW_SIZE_X / WINDOW_SIZE_Y));


    glm::mat4 model;
    
    int objColor = glGetUniformLocation(objShader.ID, "objColor");
    int lightColor = glGetUniformLocation(objShader.ID, "lightColor");
    int lightPosition = glGetUniformLocation(objShader.ID, "lightPos");
    // int viewPos = glGetUniformLocation(objShader.ID, "viewPos");
    glUniform3f(objColor, 0.1f, 0.9f, 0.2f);
    glUniform3f(lightColor, 1.0f, 1.0f, 1.0f);
    glUniform3f(lightPosition, 20, -50, 10);

    // lightShader.setVec3("lightColor",  1.0f, 1.0f, 1.0f);

    Camera::view = glm::translate(Camera::view, glm::vec3(0.0f, 0.0f, -3.0f));

    glUniformMatrix4fv(glGetUniformLocation(objShader.ID, "projection"), 1, GL_FALSE, &Camera::projection[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(objShader.ID, "model"), 1, GL_FALSE, &model[0][0]); 
    glUniformMatrix4fv(glGetUniformLocation(objShader.ID, "view"), 1, GL_FALSE, &Camera::view[0][0]);

    auto start = chrono::system_clock::now();

    /*
     *  Render loop keeps window refreshing as long
     *  as user decides to close it.
     */
    while(!glfwWindowShouldClose(Window::ID))
    {
              
        auto _st = std::chrono::system_clock::now();

        //Set the background color.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        auto _clear = std::chrono::system_clock::now();
        // float timeValue = glfwGetTime();

        // map.vertices[1] = glfwGetTime();



        Camera::view = glm::lookAt(Camera::CamPos, Camera::CamPos + Camera::CamDir, Camera::CamUp);        
        glUniformMatrix4fv(glGetUniformLocation(objShader.ID, "view"), 1, GL_FALSE, &Camera::view[0][0]);

        glUniform3f(lightPosition, 1 * sin(glfwGetTime()), -2 * sin(glfwGetTime()), 3 * cos(glfwGetTime()));
        
        auto _vie = std::chrono::system_clock::now();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffer::EBO);

        glUniform3f(objColor, 0.1f, 0.9f, 0.2f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        // glDrawElements(GL_TRIANGLES, map.Get_i_array_size(), GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // glUniform3f(objColor, 0.0f, 0.0f, 0.0f);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // glDrawElements(GL_TRIANGLES, map.Get_i_array_size(), GL_UNSIGNED_INT, 0);

        
        glClearDepth(1);

        auto _draw = std::chrono::system_clock::now();

        ++frames;
        glfwSwapBuffers(Window::ID);
        glfwPollEvents();   
        auto _poll = std::chrono::system_clock::now();
        Iopcs::processInputKey(Window::ID);
        
        auto end = std::chrono::system_clock::now();

        // cout << endl << "Clear: " << chrono::duration<double>(_clear - _st).count() << "\t";
        // cout << "View: " << chrono::duration<double>(_vie - _clear).count() << "\t";
        // cout << "Draw: " << chrono::duration<double>(_draw - _vie).count() << "\t";
        // cout << "SwapBuffers: " << chrono::duration<double>(_poll - _draw).count() << "\t";
        // cout << "End: " << chrono::duration<double>(end - _poll).count() << "\t";

    }


    cout << "Avg FPS: " << (float)frames /(glfwGetTime() - currentTime) << endl;
    cout << "Running time: " << glfwGetTime() << endl;
    /*
     *  Terminate the window to free resources.
     */
    glfwTerminate();
    return 0;
}
