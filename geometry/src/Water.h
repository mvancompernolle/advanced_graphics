#ifndef WATER_H
#define	WATER_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier
#include "Vertex.h"
#include <cmath>

// REFERENCE: ogldev.atspace.co.uk/www/tutorial22/tutorial22.html

class Water{
public:
	Water(GLint program);
	void createWaterMesh(float w, float h, float density);
	void calculateNormals();
	void render(glm::mat4 vpMat);
	std::vector<Vertex> getVertices();

private:
	//CEGUI::Window *buttonWindow;
	std::vector<Vertex> vertices;
	GLuint VBO;
	GLint program, loc_position, loc_mvpmat;
	glm::mat4 model;
};

#endif	/* WATER_H */