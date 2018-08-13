#ifndef OBJECT_HPP
#define OBJECT_HPP
#include <memory>
#include "model.hpp"

class PhysicalProp{
    protected:
    float mass;
    glm::vec3 velocity{};
    glm::vec3 position{}, rotation{};
};


class Object : protected PhysicalProp{

    public:
    std::shared_ptr<Model> model;

    Object(std::shared_ptr<Model> m){
        model = m;
    }

    void MoveObj(glm::vec3 vec){
        position += vec;
    }

    void MoveAbs(glm::vec3 vec){
        position = vec;
    }

    void Draw(unsigned int ID){
        model->Draw(ID, position);
    };
    

};

#endif