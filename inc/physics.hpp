#ifndef PHYSICS_HPP
#define PHYSICS_HPP
#include "object.hpp"


class Phy{
    public:
    float gravity = 9.81;

    static bool CheckCollision_VectorWithTerrain(glm::vec3 &Obj, float yOffset, Object terrain);
    // static bool ApplyFallingVelocity
};

#endif