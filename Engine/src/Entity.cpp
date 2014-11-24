#include "Entity.hpp"

using namespace Vancom;

Entity::Entity(){
	id = 0;
	updating = true;
}

Entity::~Entity(){

}

void Entity::scale(float scale){

	glm::scale(model, glm::vec3(scale, scale, scale));
}

glm::mat4 Entity::getModel() const{

	return model;
}

glm::vec3 Entity::getPos() const{

	return glm::vec3(model[3][0], model[3][1], model[3][2]);
}