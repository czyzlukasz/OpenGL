#include "map.hpp"


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