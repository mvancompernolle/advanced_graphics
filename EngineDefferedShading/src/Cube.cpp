#include "Cube.hpp"

using namespace Vancom;

Cube::Cube(){
}

Cube::~Cube(){

}

bool Cube::init(){

	// initalize program
	if(!program.init())
		return false;

	program.enable();

	model = glm::translate(model, glm::vec3(0,60,0));
	model = glm::scale(model, glm::vec3(10, 10, 10));

	// load cube model from assimp
	mesh = new Mesh(program);
	mesh->loadMesh("../assets/models/ballSml.obj");

	return true;
}

void Cube::tick(float dt){

}

void Cube::render(glm::mat4 projection, glm::mat4 view){

	// enable program
	program.enable();

	// enable attributes
	glEnableVertexAttribArray(program.locPos);
	glEnableVertexAttribArray(program.locTex);
	glEnableVertexAttribArray(program.locNormal);

	// set uniforms
	glm::mat4 mvp = projection * view * model;
	program.setMVP(mvp);

	// draw
	mesh->renderMesh();

	// disable attributes
	glDisableVertexAttribArray(program.locPos);
	glDisableVertexAttribArray(program.locTex);
	glDisableVertexAttribArray(program.locNormal);
}