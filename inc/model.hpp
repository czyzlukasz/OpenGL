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


    void Draw(unsigned int ID){
        for(unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // Activate proper texture unit before binding.
            if(textures[i].type == "texture_diffuse")
            glUniform1i(glGetUniformLocation(ID, "texture_diffuse1"), i);
            else if(textures[i].type == "texture_specular")
            glUniform1i(glGetUniformLocation(ID, "texture_specular1"), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].ID);
        }
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);
    }
};


/*  Model class for loading model files and converting
    to vertices, normals and indices. */
class Model{

    vector <Mesh> meshes;
    vector <Texture> loaded_textures;
    string path, filename, directory;

    //  Temporary containers for holding mesh data.
    // vector <Vertex> vertices;
    // vector <unsigned int> indices;
    // vector <Texture> textures;

    public:

    Model(string d){
        path = d;
        filename = path.substr(path.find_last_of('/') + 1);
        directory = path.substr(0, path.find_last_of('/'));

        LoadModel();
        // cout << vertices.size() << endl;
        // cout << indices.size() << endl;
        // cout << textures.size() << endl;
        // cout << meshes.size() << endl;
    }
    void Init(){
        for(auto &i : meshes){
            i.initMesh();
        }     
    }
    void Draw(unsigned int ID){
        for(auto &i : meshes){
            i.Draw(ID);
        }
    }



    private:

    //  Method for loading models from file
    void LoadModel(void){
         //  Create ASSIMP importer.
        Assimp::Importer imp;
        const aiScene *scene = imp.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
        
        //  Check for any error.
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
            cerr << "Cannot open " << filename << " from " << path << endl;
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

        for(unsigned int i = 0; i < mesh->mNumVertices; i++){
            Vertex vertex;
            //  Temporary vector.
            glm::vec3 vector;

            //  Position.
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            //  Normals.
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;

            //  Texture coords.
            if(mesh->mTextureCoords[0]){ //  Check if there is any texture coords.            
                glm::vec2 vec;

                //Take only first set of texture coordinates.
                vec.x = mesh->mTextureCoords[0][i].x; 
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TextureCoords = vec;
            }
            else vertex.TextureCoords = glm::vec2(0.0f, 0.0f);
            vertices.push_back(vertex);
        }

        //  Get indices from faces.
        for(unsigned int i = 0; i < mesh->mNumFaces; i++){
            aiFace face = mesh->mFaces[i];
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // Materials.
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        //  Diffuse textures.
        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        //  Specular textures.
        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        //  Return value by creating new Mesh instance.
        return Mesh(vertices, indices, textures);
    }

    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);


    unsigned int TextureFromFile(const char *path, const string &directory, bool gamma);
};




#endif