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

// float vertices[] = {
//     -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//      0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
//      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
//      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
//     -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
//     -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

//     -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//      0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//      0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//      0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//     -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//     -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

//     -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//     -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//     -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//     -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//     -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//     -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

//      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
//      0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//      0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//      0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//      0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
//      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

//     -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//      0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//     -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//     -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

//     -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
//      0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
//      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//     -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//     -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
// };


int main (void)
{
    //Initialise and set the window.
    Window::WindowInit();


    //Source code of vertex and fragment shaders. 
    Shader objShader("src/VertexShader", "src/FragmentShader");
    // Shader lightShader("src/LightShader", "src/FragmentShader");

    // Buffer::BufferInit();
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

    // Buffer::SetBufferData(sizeof(vertices), vertices);
    Model mmodel("models/source/TrexModelByJoel3d/TrexByJoel3d.fbx");


    glUseProgram(objShader.ID);
    // glBindVertexArray(Buffer::VAO);
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

    // auto start = chrono::system_clock::now();


    /*
     *  Render loop keeps window refreshing as long
     *  as user decides to close it.
     */
    while(!glfwWindowShouldClose(Window::ID))
    {
              
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

        Camera::view = glm::lookAt(Camera::CamPos, Camera::CamPos + Camera::CamDir, Camera::CamUp);        
        glUniformMatrix4fv(glGetUniformLocation(objShader.ID, "view"), 1, GL_FALSE, &Camera::view[0][0]);


        glUniform3f(lightPosition, 10 * sin(glfwGetTime()), 0, 30 * cos(glfwGetTime()));
        

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        mmodel.Draw();

        
        glClearDepth(1);
        ++frames;
        glfwSwapBuffers(Window::ID);
        glfwPollEvents();   
        Iopcs::processInputKey(Window::ID);
    }


    cout << "Avg FPS: " << (float)frames /(glfwGetTime() - currentTime) << endl;
    cout << "Running time: " << glfwGetTime() << endl;
    /*
     *  Terminate the window to free resources.
     */
    glfwTerminate();
    return 0;
}
