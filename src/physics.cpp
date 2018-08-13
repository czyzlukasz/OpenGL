#include <physics.hpp>


bool Phy::CheckCollision_VectorWithTerrain(glm::vec3 &Obj, float yOffset, Object terrain){
    int x, z;
    float y;

    //  Get cardinal coordinates of Object for easier calculations.
    x = Obj.x;
    y = Obj.y;
    z = Obj.z;

    //  Get terrain mesh from Object.
    vector <Vertex> terrainMesh = terrain.model->meshes[0].vertices;

    //  TO MODIFY: Hard coded map size.
    if(terrainMesh[x * 256 + z].Position.y + yOffset > y){
        //  Collision detected.
        Obj.y = terrainMesh[x * 256 + z].Position.y + yOffset;
        return true;
    }
    else return false;
}