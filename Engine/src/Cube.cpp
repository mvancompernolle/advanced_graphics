#include "Cube.hpp"

using namespace Vancom;

Cube::Cube(){
}

Cube::~Cube(){

}

bool Cube::init(){

	VertexC vert;

	// initalize program
	if(!program.init())
		return false;

	program.enable();

	// initalize geometry
	vert.color = glm::vec3(1, 0, 0);

	vert.pos = glm::vec3(-.5, .5, 0);
	geometry.push_back(vert);
	vert.pos = glm::vec3(-.5, -.5, 0);
	geometry.push_back(vert);
	vert.pos = glm::vec3(.5, -.5, 0);
	geometry.push_back(vert);
	vert.pos = glm::vec3(.5, -.5, 0);
	geometry.push_back(vert);
	vert.pos = glm::vec3(.5, .5, 0);
	geometry.push_back(vert);
	vert.pos = glm::vec3(-.5, .5, 0);
	geometry.push_back(vert);

	// create vao
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// create vbo
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexC) * geometry.size(), geometry.data(), GL_STATIC_DRAW);

	// setup attributes
	glVertexAttribPointer(program.locPos, 3, GL_FLOAT, GL_FALSE, sizeof(VertexC), 0);
	glVertexAttribPointer(program.locColor, 3, GL_FLOAT, GL_FALSE, sizeof(VertexC), (const GLvoid*)12 );

	return true;
}

void Cube::tick(float dt){

}

void Cube::render(glm::mat4 projection, glm::mat4 view){

	// enable program and bind both vao and vbo
	program.enable();
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// enable attributes
	glEnableVertexAttribArray(program.locPos);
	glEnableVertexAttribArray(program.locColor);

	// set uniforms
	glm::mat4 mvp = projection * view * model;
	program.setMVP(mvp);

	// draw
	glDrawArrays(GL_TRIANGLES, 0, geometry.size());

	// disable attributes
	glDisableVertexAttribArray(program.locPos);
	glDisableVertexAttribArray(program.locColor);
}