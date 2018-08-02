#include "model.hpp"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#endif





/*
 *
 *                      MESH
 * 
 */


void Mesh::Draw(unsigned int shaderID, glm::vec3 pos){
    for(unsigned int i = 0; i < textures.size(); ++i)
    {
        cout << textures[i].ID << " " << textures[i].path << endl;
        glActiveTexture(GL_TEXTURE0 + i); // Activate proper texture unit before binding.
        if(textures[i].type == "texture_diffuse")
        glUniform1i(glGetUniformLocation(shaderID, "texture_diffuse1"), i);
        else if(textures[i].type == "texture_specular")
        glUniform1i(glGetUniformLocation(shaderID, "texture_specular1"), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].ID);
    }
    cout << endl;
    int position_u = glGetUniformLocation(shaderID, "position");
    glUniform3fv(position_u, 1, &pos[0]);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
    // cout << "End of Draw." << endl << endl;
}

// vector <Texture> Mesh::textures{};


/*
 *
 *                      MODEL
 * 
 */


void Model::LoadModel(void){
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

void Model::processNode(const aiScene *scene, aiNode *node){
    for(unsigned int i = 0; i < node->mNumMeshes; i++){
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        meshes.push_back(processMesh(mesh, scene));			
    }
    
    for(unsigned int i = 0; i < node->mNumChildren; i++){
        processNode(scene, node->mChildren[i]);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene){
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

vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName){
    vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for(unsigned int j = 0; j < loaded_textures.size(); j++)
        {
            if(std::strcmp(loaded_textures[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(loaded_textures[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one.
                break;
            }
        }
        if(!skip)
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.ID = TextureFromFile(str.C_Str(), directory, false);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            loaded_textures.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
            cout << texture.ID << " " << texture.path << endl;
        }

    }
    return textures;
}


Texture Model::loadMaterialTextures(const char *path, const string &directory, bool gamma){
    Texture texture;
    texture.ID = TextureFromFile(path, directory, gamma);
    texture.path = path;
    texture.type = "texture_diffuse";

    cout << texture.ID << " " << texture.path << endl;
    // cout << texture.type << endl;
    loaded_textures.push_back(texture);
    return texture;
}

unsigned int Model::TextureFromFile(const char *path, const string &directory, bool gamma){
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        std::cout << "Filename: " << filename << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}