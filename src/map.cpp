#include "map.hpp"


void Map::GenerateTerrain(unsigned int map_length)
{
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
    PerlinNoise(64, 5);
    PerlinNoise(32, 3);
    PerlinNoise(16, 2);
    // PerlinNoise(16,10);


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

void Map::PerlinNoise(unsigned int grid_size, unsigned int amplitude)
{
    //Generate random gradient vectors.
    unsigned int grid_amount = map_length / grid_size + 1;
    glm::vec2 **GradGrid = new glm::vec2 *[grid_amount];
    for(unsigned int i = 0; i < grid_amount; ++i) GradGrid[i] = new glm::vec2[grid_amount];
    
    for(unsigned int i = 0; i < grid_amount; ++i)
    {
        for(unsigned int j = 0; j < grid_amount; ++j)
        {
            GradGrid[i][j] = glm::vec2((rand() % 10) / 10.0f - 0.5f, (rand() % 10) / 10.0f - 0.5f);
            // cout << i << " " << j << "Grid\n";
        }
    }
    
    // cout << "Calculating grid vectors finished\n";

    for(unsigned int z = 0; z < map_length; ++z)
    {
        for(unsigned int x = 0; x < map_length; ++x)
        {
            /*  Calculate coordinates of left corner of the square.
            *  Square is represented by 4 letters: C  D
            *                                      A  B
            */
            unsigned int grid_point_x = x / grid_size;
            unsigned int grid_point_z = z / grid_size;
            // cout << "Calculate corner values\n";
            glm::vec2 Corner_A = GradGrid[grid_point_x][grid_point_z];
            glm::vec2 Corner_B = GradGrid[grid_point_x + 1][grid_point_z];
            glm::vec2 Corner_C = GradGrid[grid_point_x][grid_point_z + 1];
            glm::vec2 Corner_D = GradGrid[grid_point_x + 1][grid_point_z + 1];

            // cout << "Calculate distance values\n";
            //Calculate distances between terrain point and each square corner.
            glm::vec2 Distance_A = glm::vec2(x, z) - glm::vec2((x / grid_size) * grid_size, (z / grid_size) * grid_size);
            glm::vec2 Distance_B = glm::vec2(x, z) - glm::vec2((x / grid_size + 1) * grid_size, (z / grid_size) * grid_size);
            glm::vec2 Distance_C = glm::vec2(x, z) - glm::vec2((x / grid_size) * grid_size, (z / grid_size + 1) * grid_size);
            glm::vec2 Distance_D = glm::vec2(x, z) - glm::vec2((x / grid_size + 1) * grid_size, (z / grid_size + 1) * grid_size);

            //Calculate dot product of Corner vectors and Distance vectors.
            //Dot products are now values that influence the terrain point.
            // cout << "Calculate dot products\n";
            float Dot_A = glm::dot(Corner_A, Distance_A) / (float) grid_size;
            float Dot_B = glm::dot(Corner_B, Distance_B) / (float) grid_size;
            float Dot_C = glm::dot(Corner_C, Distance_C) / (float) grid_size;
            float Dot_D = glm::dot(Corner_D, Distance_D) / (float) grid_size;
            // cout << Dot_A << "\n";
            // cout << Corner_A.x << " " << Corner_A.y << " :CornerA\n";
            // cout << "Coords: " << x << "  " << z << "\n";
            // cout << Distance_A.x << " " << Distance_A.y << " :DistA\n";
            // cout << Distance_B.x << " " << Distance_B.y << " :DistB\n";
            // cout << Distance_C.x << " " << Distance_C.y << " :DistC\n";
            // cout << Distance_D.x << " " << Distance_D.y << " :DistD\n\n";

            //Interpolate between each corner to get the value.
            // cout << "Calculate interpolated values\n";
            float Interp_AB = CosInterpolate(Dot_A, Dot_B, float(x % grid_size) / grid_size);
            float Interp_CD = CosInterpolate(Dot_C, Dot_D, float(x % grid_size) / grid_size);
            float Interp_Final = CosInterpolate(Interp_AB, Interp_CD, float(z % grid_size) / grid_size);
            // cout << "X: " << x << " Z: " << z << " Final value: " << Interp_Final << "\n";
            // cout << Interp_Final << "\n";
            terrain[x][z] += Interp_Final * amplitude;
        }
    }
    delete []GradGrid;
}

float Map::Interpolate (float A, float B, float alpha)
{
    // return ((B - A) / grid_size) * alpha + A;
    // cout << alpha << "ALPHA\n";
    return(A * (1 - alpha) + B * alpha);
}

float Map::CosInterpolate (float A, float B, float alpha)
{
    return Interpolate(A, B, (-cos(alpha * M_PI)/2 + 0.5f));
}

void Map::Add_vertices(float* ver, unsigned int ver_size, 
                       unsigned int *ind, unsigned int ind_size)
{
    ver_size /= sizeof(float);
    ind_size /= sizeof(unsigned int);

    unsigned int last_index = v_array_size / 3;

    v_array_size += ver_size;
    i_array_size += ind_size;


    for(unsigned int i = 0; i < ver_size; ++i)
    {
        vertices.push_back(ver[i]);
    }

    for(unsigned int i = 0; i < ind_size; ++i)
    {
        indices.push_back(ind[i] + last_index);
    }
}

void Map::Add_vertices(float* ver, unsigned int ver_size)
{
    ver_size /= sizeof(float);
    // ind_size /= sizeof(unsigned int);

    unsigned int last_index = v_array_size / 3;

    v_array_size += ver_size;
    // i_array_size += ind_size;


    for(unsigned int i = 0; i < ver_size; ++i)
    {
        vertices.push_back(ver[i]);
    }

    for(unsigned int i = 0; i < ver_size / 3; ++i)
    {
        indices.push_back(i + last_index);
        ++i_array_size;
    }
}
// void Map::Add_indices(unsigned int size)
// {
//     i_array_size += size;
//     unsigned int last_index = indices.back();
//     for(unsigned int i = 0; i < size; ++i)
//     {
//         indices.push_back(i + last_index);
//     }
// }

unsigned int Map::Get_v_array_size(void)
{
    return v_array_size;
}

unsigned int Map::Get_i_array_size(void)
{
    return i_array_size;
}

vector <float> Map::Get_vertices(void)
{
    return vertices;
}
vector <unsigned int> Map::Get_indices(void)
{
    return indices;
}