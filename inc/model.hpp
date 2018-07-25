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


    void Draw(){
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};


/*  Model class for loading model files and converting
    to vertices, normals and indices. */
class Model{

    vector <Mesh> meshes;
    string directory, filename;

    //  Temporary containers for holding mesh data.
    // vector <Vertex> vertices;
    // vector <unsigned int> indices;
    // vector <Texture> textures;



    public:

    Model(string dir){
        directory = dir;
        filename = dir.substr(dir.find_last_of('/') + 1);
        LoadModel();
        // cout << vertices.size() << endl;
        // cout << indices.size() << endl;
        // cout << textures.size() << endl;
        cout << meshes.size() << endl;
    }
    void Init(){
        for(auto &i : meshes){
        i.initMesh();
        }     
    }
    void Draw(){
        for(auto &i : meshes){
            i.Draw();
        }
    }



    private:

    //  Method for loading models from file
    void LoadModel(void){
         //  Create ASSIMP importer.
        Assimp::Importer imp;
        const aiScene *scene = imp.ReadFile(directory, aiProcess_Triangulate | aiProcess_FlipUVs);
        
        //  Check for any error.
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
            cerr << "Cannot open " << filename << " from " << directory << endl;
            cerr << imp.GetErrorString();
            return;
        }
        processNode(scene, scene->mRootNode);
    }
    
    void processNode(const aiScene *scene, aiNode *node){
        for(unsigned int i = 0; i < node->mNumMeshes; i++){
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
            meshes.push_back(processMesh(mesh, scene));			
        }
        
        for(unsigned int i = 0; i < node->mNumChildren; i++){
            processNode(scene, node->mChildren[i]);
        }
    }

    Mesh processMesh(aiMesh *mesh, const aiScene *scene){
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;
        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                Vertex vertex;
                glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
                // positions
                vector.x = mesh->mVertices[i].x;
                vector.y = mesh->mVertices[i].y;
                vector.z = mesh->mVertices[i].z;
                vertex.Position = vector;
                // normals
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
                // texture coordinates
                if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
                {
                    glm::vec2 vec;
                    // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                    // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                    vec.x = mesh->mTextureCoords[0][i].x; 
                    vec.y = mesh->mTextureCoords[0][i].y;
                    vertex.TextureCoords = vec;
                }
                else vertex.TextureCoords = glm::vec2(0.0f, 0.0f);
                vertices.push_back(vertex);
            }
            // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
            for(unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                // retrieve all indices of the face and store them in the indices vector
                for(unsigned int j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }
            return Mesh(vertices, indices, textures);
        }
};


#endif