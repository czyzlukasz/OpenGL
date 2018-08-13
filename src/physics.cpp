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

bool Phy::CheckCollision_VectorWithTerrain(Object &Obj, float yOffset, Object terrain){
    int x, z;
    float y;

    //  Get cardinal coordinates of Object for easier calculations.
    x = Obj.position.x;
    y = Obj.position.y;
    z = Obj.position.z;

    //  Get terrain mesh from Object.
    vector <Vertex> terrainMesh = terrain.model->meshes[0].vertices;

    //  TO MODIFY: Hard coded map size.
    if(terrainMesh[x * 256 + z].Position.y + yOffset > y){
        //  Collision detected.
        Obj.position.y = terrainMesh[x * 256 + z].Position.y + yOffset;
        Phy::Bounce(Obj, terrainMesh[x * 256 + z].Normal);
        return true;
    }
    else return false;
}

bool Phy::ApplyVelocity_FromGravity(Object& Obj, float deltaTime){
    Obj.velocity += glm::vec3(0, -deltaTime * Phy::gravity, 0);
    return true;
}

bool Phy::MoveObject(Object &Obj, float deltaTime){
    Obj.position += Obj.velocity * deltaTime;
    return true;
}

bool Phy::Bounce(Object &Obj, glm::vec3 normal){
    float finalVelocity = glm::length(Obj.velocity);
    
    //  Check if the bounce is big enough to avoid infinite jumping.
    if(finalVelocity <= 1){
        Obj.velocity = glm::vec3();
        return false;
    }
    glm::vec3 direction = 2.0f * glm::normalize(normal) + glm::normalize(Obj.velocity);
    Obj.velocity = direction * finalVelocity * Obj.bounciness;
    return true;
}


float Phy::gravity = 9.81;