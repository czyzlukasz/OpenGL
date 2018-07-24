#ifndef MODEL_HPP
#define MODEL_HPP

#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glad.h"
#include "glfw3.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

using namespace std;

/*  Creating struct this way will guarantee that data will be
 *  sequential in memory
 */
struct Vertex{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TextureCoords;
};

struct Texture{
    unsigned int ID;
};

class Mesh{
    //VAO - vertex array object.
    static unsigned int VAO;
    //VBO - vertex buffer object.
    static unsigned int VBO;
    //EBO - element buffer object.
    static unsigned int EBO;


    public:
    //Containers for holding mesh data.
    vector <Vertex> vertices;
    vector <unsigned int> indices;
    vector <Texture> textures;


    Mesh(vector <Vertex> v, vector <unsigned int> i, vector <Texture> t){
        vertices = v;
        indices = i;
        textures = t;
    }


    void initMesh(){
        //Generate buffers.
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        //Bind buffers.
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        //Send buffer to GPU.
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        //Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);

        //Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));

        //Texture attribute
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));


        //Enable attributes, so they can be passed to shaders.
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glBindVertexArray(0); 
    }


    void Draw(){
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};


/*  Model class for loading model files and converting
    to vertices, normals and indices. */
class Model{

    Model(string directory){
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(directory, aiProcess_Triangulate | aiProcess_FlipUVs);

        //  Check for any errors.
        if(importer.GetErrorString()){
            std::cerr << importer.GetErrorString() << std::endl;
        }
    }
};


#endif