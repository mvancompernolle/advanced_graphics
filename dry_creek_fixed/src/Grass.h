#ifndef GRASS_H
#define	GRASS_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier
#include <vector>
#include "Vertex.h"
#include <random>

class Grass
{
public:
    Grass(GLuint program, float samplePercent, float heightScalar, float widthScalar, glm::vec3 color);
    ~Grass();
    void sampleVertices(const std::vector<VertexTexture>& vertices);
    void render(glm::mat4 pvMat);

private:
	float percent, heightScalar, widthScalar;
	glm::vec3 color;
	std::vector<VertexGrass> vertices;
	GLuint VBO;
    GLuint program, loc_position, loc_color, loc_hscalar, loc_mvp, loc_wscalar, loc_gcolor, loc_offset;
    glm::mat4 model;
};


#endif	/* GRASS_H */

