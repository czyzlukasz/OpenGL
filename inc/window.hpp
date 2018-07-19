#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <fstream>
#include <sstream>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glad.h"
#include "glfw3.h"

using namespace std;

// Size of the window.
#define WINDOW_SIZE_X 1280   
#define WINDOW_SIZE_Y 1024

// Enable or disable VSync.
#define ENABLE_VSYNC 1


GLFWwindow* SetWindow();
void FrameBufferSize(GLFWwindow* window, int width, int height);
//void processInputKey(GLFWwindow *window);
/*
 *  Class holding vertex and fragment shaders.
 *  Constructor requires passing paths to two files
 *  containing shaders' code.
 */
class Shader
{
    public:
    unsigned int ID;

    private:
    string vertexCode, fragmentCode;
    ifstream file_vertexCode, file_fragmentCode;

    public:
    Shader(const char* vertexPath, const char* fragmentPath)
    {
        std::stringstream StreamVertex, StreamFragment;

        //Try to open both files containing shaders' code.
        //Copy contents of steams to the corresponding strings.
        file_vertexCode.open(vertexPath);
        if(file_vertexCode.is_open())
        {
            StreamVertex << file_vertexCode.rdbuf();
            vertexCode = StreamVertex.str();
        }


        file_fragmentCode.open(fragmentPath);
        if(file_fragmentCode.is_open())
        {
            StreamFragment << file_fragmentCode.rdbuf();
            fragmentCode = StreamFragment.str();
        }



        //Close files.
        file_vertexCode.close();
        file_fragmentCode.close();

        /*  Convert strings to const char* type to allow GLSL accept
         *  the code.
         */
        const char* CvertexCode = vertexCode.c_str();
        const char * CfragmentCode = fragmentCode.c_str();
        
        //Handlers holding Shaders' ID.
        unsigned int vertexShader, fragmentShader;


        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        
        //Link the ID and code of shaders.
        glShaderSource(vertexShader, 1, &CvertexCode, NULL);
        glShaderSource(fragmentShader, 1, &CfragmentCode, NULL);
        
        //Compile vertex and fragment shaders.
        glCompileShader(vertexShader);
        glCompileShader(fragmentShader);


        //Check if compilation went ok.
        int successVertex, successFragment;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successVertex);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &successFragment);

        if(!successVertex) 
        {
            char errorlog[250];
            glGetShaderInfoLog(vertexShader, sizeof(errorlog), NULL, errorlog);
            std::cout << "Vertex Shader compilation error!" << std::endl << errorlog << std::endl;
        }
        if(!successFragment) 
        {
            char errorlog[250];
            glGetShaderInfoLog(fragmentShader, sizeof(errorlog), NULL, errorlog);
            std::cout << "Vertex Shader compilation error!" << std::endl << errorlog << std::endl;
        }
        ID = glCreateProgram();
        glAttachShader(ID, vertexShader);
        glAttachShader(ID, fragmentShader);
        glLinkProgram(ID);

        int successProgram;
        glGetProgramiv(ID, GL_LINK_STATUS, &successProgram);

        if(!successProgram) 
        {
            char errorlog[250];
            glGetProgramInfoLog(ID, sizeof(errorlog), NULL, errorlog);
            cout << "Program linking error! " << successProgram << endl << errorlog << endl;
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        
    }
};

class Window
{
    public:
    GLFWwindow *ID;
    Window()
    {
        glfwInit(); 
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 16);


        ID = glfwCreateWindow(WINDOW_SIZE_X, WINDOW_SIZE_Y, "OpenGL", NULL, NULL);
        
        if (ID == NULL)    
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
        }
        glfwMakeContextCurrent(ID);


        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
        } 
        glViewport(0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y);
        glfwSetFramebufferSizeCallback(ID, FrameBufferSize);
        
        //Disable/Enable VSync.
        glfwSwapInterval(ENABLE_VSYNC);

        //Enable depth test to avoid clipping textures.
        glEnable(GL_DEPTH_TEST);

        //Set drawing style to just lines.
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        //Set cursor properties.
        glfwSetInputMode(ID, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  

    }
};

class Buffer
{
    public:

    //VAO - vertex array object.
    static unsigned int VAO;
    //VBO - vertex buffer object.
    static unsigned int VBO;
    //EBO - element buffer object.
    static unsigned int EBO;

    static void SetBufferData(unsigned int, std::vector <float>, unsigned int, std::vector <unsigned int>);

    static void BufferInit()
    {
        //Generate buffers.
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
    }
};


#endif