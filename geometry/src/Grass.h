#ifndef TEXTURE_H
#define	TEXTURE_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier
#include <vector>
#include "Vertex.h"
#include <random>

// REFERENCE: ogldev.atspace.co.uk/www/tutorial16/tutorial16.html

class Grass
{
public:
    Grass(GLuint program, float samplePercent, glm::vec3 color);
    ~Grass();
    void sampleVertices(const std::vector<Vertex>& vertices);
    void render(glm::mat4 pvMat);

private:
	float percent;
	glm::vec3 color;
	std::vector<Vertex> vertices;
	GLuint VBO;
    GLuint program, loc_position, loc_color, loc_hscalar, loc_mvp;
    glm::mat4 model;
};


#endif	/* TEXTURE_H */

