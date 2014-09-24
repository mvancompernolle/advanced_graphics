#ifndef VERTEX_H
#define	VERTEX_H
#include <GL/glew.h>

struct Vertex
{
    GLfloat pos[3];
};

struct VertexColor : Vertex {
	GLfloat samplePos;
};

#endif	/* VERTEX_H */
