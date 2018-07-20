#ifndef MAP_HPP
#define MAP_HPP

#include <iostream>
#include <cstdlib>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/vec2.hpp"

using namespace std;


class Map
{    
    private:
    //Vectors used to render map.
    vector <float> vertices;
    vector <unsigned int> indices;

    //Variables holding value properties needed to generate buffer.
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

        // PerlinNoise(512, 150);
        // PerlinNoise(256, 110);
        // PerlinNoise(128, 30);
        // PerlinNoise(64, 5);
        PerlinNoise(32, 3);
        PerlinNoise(16, 2);
        //Set and allocate the size of vertices array.
        v_array_size = map_length * map_length * 3;

        for(unsigned int i = 0; i < map_length * map_length; ++i)
        {
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

#endif