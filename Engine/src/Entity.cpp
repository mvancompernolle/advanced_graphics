#include "Entity.hpp"

using namespace Vancom;

Entity::Entity(){
	id = 0;
}

Entity::~Entity(){

}

void Entity::scale(float scale){

	glm::scale(model, glm::vec3(scale, scale, scale));
}

glm::mat4 Entity::getModel() const{

	return model;
}