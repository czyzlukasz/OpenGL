#include <iostream>
#include "window.hpp"



    /*
     * Function for resizing the window.
     */
void FrameBufferSize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  


void Buffer::SetBufferData(unsigned int size_v, std::vector <float> vertices, unsigned int size_i, std::vector <unsigned int> indices)
{
    //Passing vector as a address of first element is needed by GLFW,
    //Because it was written in ANSI C.
    glBufferData(GL_ARRAY_BUFFER, size_v, &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_i, &indices[0], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    // normal attributes
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    glBindVertexArray(0); 
    // glBindVertexArray(1); 
    glDisableVertexAttribArray(0);
}

void Buffer::SetBufferData(unsigned int size_v, float vertices[])
{
    //Passing vector as a address of first element is needed by GLFW,
    //Because it was written in ANSI C.
    glBufferData(GL_ARRAY_BUFFER, size_v, vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    // normal attributes
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    glBindVertexArray(0); 
    glDisableVertexAttribArray(0);
}

unsigned int Buffer::VAO{};
unsigned int Buffer::VBO{};
unsigned int Buffer::EBO{};



GLFWwindow *Window::ID = nullptr;