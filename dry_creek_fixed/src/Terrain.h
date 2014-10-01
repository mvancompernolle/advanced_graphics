#ifndef TERRAIN_H
#define	TERRAIN_H

#include <vector>
#include "Texture.h"
#include "Vertex.h"
#include "Shape.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier
#include <ogr_spatialref.h>
#include "ogr_srs_api.h"
#include <random>

// REFERENCE: ogldev.atspace.co.uk/www/tutorial22/tutorial22.html

class Terrain{
public:
	Terrain(GLint program, const char* fileName);
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
	void setScale(float heightScale, float scale);
	bool generateMesh();
	void cutSquareHole(float startX, float startZ, float xSize, float zSize);
	void geoTransform(GDALDataset* projDataSet, double& x, double& y);
	void renderDataZone(glm::mat4 projection, glm::mat4 view);
	void render(glm::mat4 projection, glm::mat4 view);
	void buttonPressed();
	double* getGeot();
	const std::vector<VertexTexture>& getVertices();
	GDALDataset* getGdalDataset();
	void relativeTranslate(glm::vec3 vec3);
	void setMinMax(float min, float max);
	float getMin();
	float getMax();
	void addShape(const char* fileName, GLint program, glm::vec3 color, float isBoundary);
	void placeShapesOnSurface();
	void renderShapes(glm::mat4 projection, glm::mat4 view);
	void setMaskTexture(Texture* tex);
	void useGrayScaleTexture();
	void colorSurfaceWithShapes();

private:
	void registerForEvents();
	bool checkDataZone(float, float, float);
	void parseDataFileName(const char* fileName);

	//CEGUI::Window *buttonWindow;
	std::vector<Shape*> shapes;
	bool loaded;
	Texture* heightMapImage, *dataZoneDataImage, *dataTexture;
	GDALDataset* gdalDataZone, *gdalImage;
	GDALRasterBand  *poBandDataZone, *poBand;
	std::vector<VertexTexture> planeVertices;
	std::vector<VertexColor> dataZoneVertices;
	double* geot;
	int width, height, numDataBands, currentDataBand, bandXSize, currentTime;
	std::string directoryAndName, extension;
	float scale, heightScale, min, max, range;
	glm::mat4 model;
	GLint program, loc_position, loc_scalar, loc_sampler, loc_mvpmat, loc_texture;
	GLint dataZoneProgram, dz_loc_position, dz_loc_scalar, dz_loc_sampler, dz_loc_mvpmat, dz_loc_samplepos;
	GLuint VBO, dzVBO;
};

#endif	/* TERRAIN_H */