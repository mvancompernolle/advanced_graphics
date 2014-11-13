#include "Model.hpp"

using namespace Vancom;

Model::Model(){

	specularIntensity = 0;
	specularPower = 0;
}

Model::Model(glm::vec3 pos, float scale, float power, float intensity){
	
	Model();
	specularIntensity = power;
	specularPower = intensity;	
	model = glm::translate(model, pos);
	model = glm::scale(model, glm::vec3(scale, scale, scale));
}

Model::~Model(){

}

bool Model::init(const char* fileName){

	// load Model model from assimp
	mesh = new Mesh();
	mesh->loadMesh(fileName, true);

	return true;
}

void Model::tick(float dt){

}

void Model::render(){

	// draw
	mesh->renderMesh();
}