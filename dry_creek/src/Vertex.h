#ifndef VERTEX_H
#define	VERTEX_H

struct Vertex
{
    GLfloat position[3];
};

struct VertexColor : Vertex {
	GLfloat samplePos;
};

#endif	/* VERTEX_H */
