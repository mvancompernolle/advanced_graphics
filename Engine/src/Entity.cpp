#include "Entity.hpp"

using namespace Vancom;

Entity::Entity(){
}

Entity::~Entity(){

}

void Entity::scale(float scale){

	glm::scale(model, glm::vec3(scale, scale, scale));
}