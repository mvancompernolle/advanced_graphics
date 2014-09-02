#ifndef TERRAIN_H
#define	TERRAIN_H

#include <vector>
#include "Texture.h"
#include "Vertex.h"

// REFERENCE: ogldev.atspace.co.uk/www/tutorial22/tutorial22.html

class Terrain{
public:
	Terrain();
	int getWidth();
	int getHeight();
	bool loadHeightMap(const char* fileName, float scale);
	bool generateMesh();
	void render(GLint loc_position, GLint loc_normal);

private:
	Texture* heightMapImage;
	std::vector<Vertex> planeVertices;
	int width, height;
	float scale;
	GLuint VBO;
	int numTriangles;
};

#endif	/* TERRAIN_H */
