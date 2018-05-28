#ifndef MAP_HPP
#define MAP_HPP

#pragma once

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
    public: //CHANGE TO PRIVATE!!!
    //Vectors used to render map.
    vector <float> vertices;
    vector <unsigned int> indices;

    //Variables holding value properties needed to generate buffer.
    unsigned int v_array_size = 0;
    unsigned int i_array_size = 0;
    unsigned int map_length = 0;
    unsigned int indices_size = 0; 


    public:
    vector <vector <float>> terrain;


    private:
    void GenerateTerrain(unsigned int);
    void PerlinNoise(unsigned int, unsigned int amplitude = 1);
    float Interpolate (float, float, float);
    float CosInterpolate (float, float, float);


    public:    
    void Add_vertices(float* , unsigned int, unsigned int*, unsigned int);
    void Add_vertices(float* , unsigned int);
    // void Add_indices(unsigned int);
    unsigned int Get_v_array_size(void);
    unsigned int Get_i_array_size(void);
    vector <float> Get_vertices(void);
    vector <unsigned int> Get_indices(void);

    Map(unsigned int len = 16)
    {
        map_length = len;

        //Set and allocate the size of vertices array.
        v_array_size = map_length * map_length * 3;
        //Set and allocate the size of indices array.
        i_array_size = (map_length - 1) * (map_length - 1) * 6;
        indices_size = map_length - 1;
        GenerateTerrain(map_length);
    }
};

#endif