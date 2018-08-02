#ifndef MODEL_HPP
#define MODEL_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>



#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glad.h"
#include "glfw3.h"


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
    //  Pathes of the textures allows for re-using one texture
    //  on multiple faces.
    string path;
    string type;
    unsigned int ID;
};


class Mesh{
    //VAO - vertex array object.
    unsigned int VAO;
    //VBO - vertex buffer object.
    unsigned int VBO;
    //EBO - element buffer object.
    unsigned int EBO;


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
        glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        //Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

        //Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

        //Texture attribute
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));


        //Enable attributes, so they can be passed to shaders.
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glBindVertexArray(0); 
    }


    void Draw(unsigned int ID, glm::vec3 pos = glm::vec3(0, 0, 0));
};


/*  Model class for loading model files and converting
    to vertices, normals and indices. */
class Model{
    public:

    vector <Mesh> meshes;
    vector <Texture> loaded_textures;
    string path, filename, directory;

    //  Temporary containers for holding mesh data.
    // vector <Vertex> vertices;
    // vector <unsigned int> indices;
    // vector <Texture> textures;

    Model(string d){
        path = d;
        filename = path.substr(path.find_last_of('/') + 1);
        directory = path.substr(0, path.find_last_of('/'));

        LoadModel();
        // cout << vertices.size() << endl;
        // cout << indices.size() << endl;
        // cout << textures.size() << endl;
        // cout << filename << meshes.size() << endl;
        // cout << loaded_textures.size() << endl;
    }

    Model(Mesh m, string d){
        path = d;
        filename = path.substr(path.find_last_of('/') + 1);
        directory = path.substr(0, path.find_last_of('/'));
        m.textures.push_back( loadMaterialTextures(filename.c_str(), directory.c_str(), false) );
        meshes.push_back(m);
    }

    void Init(){
        for(auto &i : meshes){
            i.initMesh();
        }     
    }

    void Draw( unsigned int ID, glm::vec3 position = glm::vec3(0, 0, 0) ){
        // cout << "STARTING OF DRAW" << endl;
        for(auto &i : meshes){
            i.Draw(ID, position);
        }
        // cout << "ENDING OF DRAW" << endl;
    }



    //  Method for loading models from file
    void LoadModel(void);
    
    void processNode(const aiScene *scene, aiNode *node);

    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);

    Texture loadMaterialTextures(const char *path, const string &directory, bool gamma);

    unsigned int TextureFromFile(const char *path, const string &directory, bool gamma);
};




#endif