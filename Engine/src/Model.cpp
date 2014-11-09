#include "Model.hpp"

using namespace Vancom;

Model::Model(){

	specularIntensity = .5;
	specularPower = 1;
}

Model::Model(glm::vec3 pos, float scale){
	
	Model();
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

	// enable attributes
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// draw
	mesh->renderMesh();

	// disable attributes
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}