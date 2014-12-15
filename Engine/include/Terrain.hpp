#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include <GL/glew.h>

#include <vector>
#include <ogr_spatialref.h>
#include <ogr_srs_api.h>
#include <gdal_priv.h>
#include <btBulletDynamicsCommon.h>

#include "TerrainProgram.hpp"
#include "Entity.hpp"
#include "Vertex.hpp"
#include "Texture.hpp"

namespace Vancom{

class Engine;

class Terrain : public Entity{

public:

	// public functions
	Terrain(Engine *engine, const char* fileName, float heightScale, float widthScale);
	~Terrain();

	bool init();
	void tick(float dt);
	void render();

	bool setTexture(GLenum TextureTarget, const char* fileName);

	void getDimensions(int& width, int& height) const;
	float** getHeightMap() const;
	std::vector<VertexTN> getGeometry() const;

private:

	// private functions
	bool generateMesh();
	void calculateNormal(float **data, int z, int x, VertexTN& vert);

	// private variables
	Engine *engine;
    const char* fileName;
	std::vector<VertexTN> geometry;
	std::vector<unsigned int> indices;
	GLuint ibo;
	int width, height, downSample;
	float min, max, range;
	Texture* groundTexture;
	float heightScale, widthScale;

	GDALDataset* gdalDataSet;
	double* geotransform;

	// bullet variables
	btTriangleMesh* trimesh;
	btCollisionShape* shape;
	btRigidBody* rigidBody;
};

} // end Vancom namepsace

#endif // END TERRAIN_HPP