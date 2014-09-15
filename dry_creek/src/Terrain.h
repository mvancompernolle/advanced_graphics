#ifndef TERRAIN_H
#define	TERRAIN_H

#include <vector>
#include "Texture.h"
#include "Vertex.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

// REFERENCE: ogldev.atspace.co.uk/www/tutorial22/tutorial22.html

class Terrain{
public:
	Terrain(GLint program);
	int getWidth();
	int getHeight();
	void setDataZone(const char* fileName, GLint program);
	void setDataZoneData(const char* fileName);
	bool loadHeightMap(const char* fileName, float heightScale, float scale);
	bool generateMesh();
	void renderDataZone(glm::mat4 projection, glm::mat4 view);
	void render(glm::mat4 projection, glm::mat4 view);
	void buttonPressed();

private:
	void registerForEvents();
	bool checkDataZone(float, float, float);

	//CEGUI::Window *buttonWindow;
	Texture* heightMapImage;
	GDALDataset* gdalDataZone;
	GDALRasterBand  *poBandDataZone;
	std::vector<Vertex> planeVertices;
	std::vector<VertexColor> dataZoneVertices;
	int width, height;
	float scale, heightScale;
	glm::mat4 model;
	GLint program, loc_position, loc_scalar, loc_sampler, loc_mvpmat;
	GLint dataZoneProgram, dz_loc_position, dz_loc_scalar, dz_loc_sampler, dz_loc_mvpmat, dz_loc_samplepos;
	GLuint VBO, dzVBO;
};

#endif	/* TERRAIN_H */