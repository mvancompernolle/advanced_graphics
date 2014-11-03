#include "Cube.hpp"

#include <iostream>

using namespace Vancom;

Cube::Cube(glm::vec3 pos){

	model = glm::translate(model, pos);
}

Cube::~Cube(){

}

bool Cube::init(float size){

	VertexTN vert;

	// initalize geometry
	vert.normal = glm::vec3(0, 1, 0);
	vert.pos = glm::vec3(-size, 0, size);
	vert.tex = glm::vec2(0,0);
	geometry.push_back(vert);
	vert.pos = glm::vec3(-size, 0, -size);
	vert.tex = glm::vec2(0,1);
	geometry.push_back(vert);
	vert.pos = glm::vec3(size, 0, size);
	vert.tex = glm::vec2(1,1);
	geometry.push_back(vert);
	vert.pos = glm::vec3(size, 0, -size);
	vert.tex = glm::vec2(1,0);
	geometry.push_back(vert);

	// create vbo
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexTN) * geometry.size(), geometry.data(), GL_STATIC_DRAW);

	// setup attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTN), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexTN), (const GLvoid*)12 );
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTN), (const GLvoid*)20 );

	return true;
}

void Cube::tick(float dt){

}

void Cube::render(glm::mat4 projection, glm::mat4 view){

}

void Cube::render(glm::mat4 projection, glm::mat4 view, ShadowMapProgram program){
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// enable attributes
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// set uniforms
	glm::mat4 mvp = projection * view * model;
	program.setMVP(mvp);

	// draw
	glDrawArrays(GL_TRIANGLE_STRIP, 0, geometry.size());
	
	// disable attributes
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

}