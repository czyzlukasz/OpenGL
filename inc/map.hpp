#ifndef MAP_HPP
#define MAP_HPP

#include <iostream>
#include <cstdlib>
#include <vector>
#include <memory>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/vec2.hpp"
#include "object.hpp"

using namespace std;

class Map{

    public:
    vector <vector <float>> terrain;
    unique_ptr<Mesh> mesh;
    unsigned int map_length;

    Map(unsigned int size){
        map_length = size;
        terrain.resize(map_length);

        for(auto &i : terrain){
            i.resize(map_length);
        }

        for(int i = map_length; i > 0; i /= 2){
            PerlinNoise( i, pow(i, 2) / (2 * map_length) );
        }
        // PerlinNoise(512, 128);
        // PerlinNoise(256, 64);
        // PerlinNoise(128, 32);
        // PerlinNoise(64, 16);
        // PerlinNoise(32, 2);
        // // PerlinNoise(16, 1);
        // mesh = make_unique<Mesh>( TerrainToMesh() );
    }


    //  Methods.
    void PerlinNoise(unsigned int grid_size, unsigned int amplitude);
    float CosInterpolate (float A, float B, float alpha);
    float Interpolate (float A, float B, float alpha);
    Mesh TerrainToMesh(void);
};

/*  Old class.
 *
class Map
{    
    private:
    //Vectors used to render map.
    vector <float> vertices;
    vector <unsigned int> indices;

    //Variables holding value properties' values needed to generate buffer.
    unsigned int v_array_size;
    unsigned int i_array_size;
    unsigned int map_length;


    public:
    vector <vector <float>> terrain;


    private:
    void PerlinNoise(unsigned int, unsigned int amplitude = 1);
    float Interpolate (float, float, float);
    float CosInterpolate (float, float, float);


    public:    
    unsigned int Get_v_array_size(void);
    unsigned int Get_i_array_size(void);
    vector <float> Get_vertices(void);
    vector <unsigned int> Get_indices(void);

    Map(unsigned int len = 32)
    {
        map_length = len;
        
        //Set and allocate the size of terrain 2D array.       
        for(unsigned int i = 0; i < map_length; ++i)
        {
            vector <float> coords;
            for(unsigned int j = 0; j < map_length; ++j)
            {
                coords.push_back(0);
            }
            terrain.push_back(coords);
        }

        PerlinNoise(512, 150);
        PerlinNoise(256, 110);
        PerlinNoise(128, 30);
        PerlinNoise(64, 10);
        PerlinNoise(32, 8);
        PerlinNoise(16, 5);
        //Set and allocate the size of vertices array.
        v_array_size = map_length * map_length * 6;

        for(unsigned int i = 0; i < map_length * map_length; ++i){
            //Local variable for holding XZ coords.
            unsigned int x = i / map_length;
            unsigned int z = i % map_length;
            // cout << "X: " << x << " Z: " << z << "\n";

            vertices.push_back(x);
            vertices.push_back(terrain[x][z]);
            vertices.push_back(z);

        }

        //Set and allocate the size of indices array.
        i_array_size = (map_length - 1) * (map_length - 1) * 6;
        
        unsigned int indices_size = map_length - 1; 

        for(unsigned int i = 0; i < i_array_size; i += 6)
        {
            unsigned int k = i / 6;

            indices.push_back((k % indices_size) + (k / indices_size) * map_length);
            indices.push_back((k % indices_size) + (k / indices_size) * map_length + 1);

            indices.push_back(indices[i] + map_length);
            indices.push_back(indices[i + 2]);
            indices.push_back(indices[i + 2] + 1);
            indices.push_back(indices[i + 1]);
        }
    }
};
*/
#endif