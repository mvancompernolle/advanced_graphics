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
	~Terrain();
	int getWidth();
	int getHeight();
	void setDataZone(const char* fileName, GLint program);
	bool setDataZoneData(const char* fileName, bool persistantBand);
	bool setDataZoneBand(int num);
	void showNextDataTime();
	void showPrevDataTime();
	void showNextDataBand();
	void showPrevDataBand();
	bool loadHeightMap(const char* fileName, float heightScale, float scale);
	bool generateMesh();
	void renderDataZone(glm::mat4 projection, glm::mat4 view);
	void render(glm::mat4 projection, glm::mat4 view);
	void buttonPressed();

private:
	void registerForEvents();
	bool checkDataZone(float, float, float);
	void parseDataFileName(const char* fileName);

	//CEGUI::Window *buttonWindow;
	Texture* heightMapImage, *dataZoneDataImage;
	GDALDataset* gdalDataZone;
	GDALRasterBand  *poBandDataZone;
	std::vector<Vertex> planeVertices;
	std::vector<VertexColor> dataZoneVertices;
	int width, height, numDataBands, currentDataBand, bandXSize, currentTime;
	std::string directoryAndName, extension;
	float scale, heightScale;
	glm::mat4 model;
	GLint program, loc_position, loc_scalar, loc_sampler, loc_mvpmat;
	GLint dataZoneProgram, dz_loc_position, dz_loc_scalar, dz_loc_sampler, dz_loc_mvpmat, dz_loc_samplepos;
	GLuint VBO, dzVBO;
};

#endif	/* TERRAIN_H */