#include <iostream>
#include <cstring>
#include <cmath>
#include <memory>
#include <cstdlib>
#include <chrono>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "camera.hpp"
#include "window.hpp"
#include "ioprocess.hpp"
#include "object.hpp"

using namespace std;


int main (void)
{
    //Initialise and set the window.
    Window::WindowInit();


    //Source code of vertex and fragment shaders. 
    Shader objShader("src/VertexShader", "src/FragmentShader");


    //Set callback functions for processing mouse inputs.
    glfwSetCursorPosCallback(Window::ID, Iopcs::processInputMouse);
    glfwSetScrollCallback(Window::ID, Iopcs::processInputScroll); 

    int frames = 0;
    float currentTime = glfwGetTime();

    //  Load new model.
    shared_ptr<Model> t_rex_mod = make_shared <Model>("models/source/TrexModelByJoel3d/TrexByJoel3d.fbx");

    //  Set up new objects.
    // Object t_rex(t_rex_mod);


    //  Initialise model.
    t_rex_mod->Init();

    vector<Object> t_rexes;
    int siz = 20;
    while(--siz){
        t_rexes.push_back(Object(t_rex_mod));
    }

    glUseProgram(objShader.ID);
    
    Camera::SetProjection((float)(WINDOW_SIZE_X / WINDOW_SIZE_Y));


    glm::mat4 model;
    
    //  Get light uniforms.
    int lightColor = glGetUniformLocation(objShader.ID, "lightColor");
    int lightPosition = glGetUniformLocation(objShader.ID, "lightPos");
    glUniform3f(lightColor, 1.0f, 1.0f, 1.0f);
    glUniform3f(lightPosition, 20, -50, 10);


    Camera::view = glm::translate(Camera::view, glm::vec3(0.0f, 0.0f, -3.0f));

    glUniformMatrix4fv(glGetUniformLocation(objShader.ID, "projection"), 1, GL_FALSE, &Camera::projection[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(objShader.ID, "model"), 1, GL_FALSE, &model[0][0]); 
    glUniformMatrix4fv(glGetUniformLocation(objShader.ID, "view"), 1, GL_FALSE, &Camera::view[0][0]);


    //Init model.
    // t_rex_mod->Init();
    // for(auto &i : t_rexes){
    //     i.model->Init();
    // }

    for(auto &i : t_rexes){
        i.MoveAbs( glm::vec3( rand()%100 - 50, 0, rand()%100 - 50 ) );
    }
    /*
     *  Render loop keeps window refreshing as long
     *  as user decides to close it.
     */
    while(!glfwWindowShouldClose(Window::ID))
    {
              
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);


        //  Calculate the view matrix based on camera view.
        Camera::view = glm::lookAt(Camera::CamPos, Camera::CamPos + Camera::CamDir, Camera::CamUp);        
        glUniformMatrix4fv(glGetUniformLocation(objShader.ID, "view"), 1, GL_FALSE, &Camera::view[0][0]);

        // Set the light position.
        glm::vec3 lp = glm::vec3(30 * sin(2 * glfwGetTime()), 0, 30 * cos(2 * glfwGetTime()));
        glUniform3fv(lightPosition, 1, &lp[0]);      


        // t_rex.MoveAbs(glm::vec3(4 * sin( glfwGetTime() ), 0, 0));

        //Draw model.
        // t_rex.Draw(objShader.ID);
        for(auto &i : t_rexes){
            i.Draw(objShader.ID);
        }


        
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
