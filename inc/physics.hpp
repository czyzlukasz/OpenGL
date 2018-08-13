#ifndef PHYSICS_HPP
#define PHYSICS_HPP
#include "object.hpp"


class Phy{
    public:
    static float gravity;

    static bool CheckCollision_VectorWithTerrain(glm::vec3 &Obj, float yOffset, Object terrain);
    static bool CheckCollision_VectorWithTerrain(Object &Obj, float yOffset, Object terrain);
    static bool ApplyVelocity_FromGravity(Object& Obj, float deltaTime);
    static bool MoveObject(Object &Obj, float deltaTime);
    static bool Bounce(Object &Obj, glm::vec3 direction);
};

#endif