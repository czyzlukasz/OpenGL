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
                GradGrid[i][j] = glm::vec2((rand() % 1000) / 1000.0f - 0.5f, (rand() % 1000) / 1000.0f - 0.5f);
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
                terrain[x][z] += (Interp_Final * amplitude);
            }
        }
        delete []GradGrid;

    }

float Map::Interpolate (float A, float B, float alpha)
{
    return(A * (1 - alpha) + B * alpha);
}

float Map::CosInterpolate (float A, float B, float alpha)
{
    return Interpolate(A, B, (-cos(alpha * M_PI)/2 + 0.5f));
}

Mesh Map::TerrainToMesh(void){
    vector <Vertex> vertices;
    vector <unsigned int> indices;
    vector <Texture> textures;


    //  Put all Position and Texture coordinates to vertices vector.
    for(uint i = 0; i < map_length; ++i){
        for(uint j = 0; j < map_length; ++j){
            Vertex temp;
            temp.Position = glm::vec3(i, terrain[i][j], j);
            temp.TextureCoords = glm::vec2(i % 2, j % 2);
            vertices.push_back(temp);
        }
    }

    //  Calculate normals.
    for(uint x = 0; x < map_length; ++x){
        for(uint z = 0; z < map_length; ++z){
            //  Obtain (if possible) two cardinal points around terrain[x][z].
            glm::vec3 N, E, S, W;

            //  N
            int idx = x * map_length + z + 1;
            if(idx < vertices.size())   N = vertices[idx].Position;

            //  E
            idx = x * map_length + z + map_length;
            if(idx < vertices.size())   E = vertices[idx].Position;

            // //  S
            // idx = x * map_length + z - 1;
            // if(idx < vertices.size())   S = vertices[idx].Position;

            // //  W
            // idx = x * map_length + z - map_length;
            // if(idx < vertices.size())   W = vertices[idx].Position;

            // cout << x << " " << z << "\t\t" << E.x << " " << E.y << " " << E.z << endl;
            // cout << x << " " << z << "\t\t" << W.x << " " << W.y << " " << W.z << endl << endl;

            //  Calculate vectors from [x][z] to N and [x][z] to E.
            uint curr_idx = x * map_length + z;
            //  N
            if( N != glm::vec3(0, 0, 0) ){
                N -= vertices[curr_idx].Position;
            }
            else{
                N = glm::vec3(0, 0, 1);
            }

            //  E
            if( E != glm::vec3(0, 0, 0) ){
                E -= vertices[curr_idx].Position;
            }
            else{
                E = glm::vec3(1, 0, 0);
            }

            vertices[curr_idx].Normal = glm::normalize( glm::cross(N, E) );
            // cout << vertices[curr_idx].Normal.x << " " <<
            //         vertices[curr_idx].Normal.y << " " <<
            //         vertices[curr_idx].Normal.z << endl;
        }
    }

    //Set and allocate the size of indices array.
    uint i_array_size = (map_length - 1) * (map_length - 1) * 6;
    
    unsigned int indices_size = map_length - 1; 

    for(unsigned int i = 0; i < i_array_size; i += 6)
    {
        unsigned int k = i / 6;

        indices.push_back( (k % indices_size) + (k / indices_size) * map_length );
        indices.push_back( (k % indices_size) + (k / indices_size) * map_length + 1 );
        indices.push_back( indices[i] + map_length );

        indices.push_back( indices[i + 2] );
        indices.push_back( indices[i + 2] + 1 );
        indices.push_back( indices[i + 1] );
    }
    // for(auto &i : indices) cout << i << endl;
    return Mesh(vertices, indices, textures);
}