#ifndef VERTEX_H
#define	VERTEX_H

struct Vertex
{
    GLfloat position[3];
};

struct VertexColor : Vertex {
	GLfloat samplePos;
};

struct VertexTexture : Vertex {
	GLfloat texture[2];
};

struct VertexGrass : Vertex {
	GLfloat color[3];
	GLfloat offset;
};

#endif	/* VERTEX_H */
