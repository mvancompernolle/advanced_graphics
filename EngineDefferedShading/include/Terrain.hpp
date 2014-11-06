#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include <GL/glew.h>

#include <vector>
#include <ogr_spatialref.h>
#include <ogr_srs_api.h>
#include <gdal_priv.h>

#include "TerrainProgram.hpp"
#include "DirectionalLightDSProgram.hpp"
#include "Entity.hpp"
#include "Vertex.hpp"
#include "Texture.hpp"
#include "GBuffer.hpp"
#include "Mesh.hpp"

namespace Vancom{

class Engine;

class Terrain : public Entity{

public:

	// public functions
	Terrain(Engine *engine, const char* fileName);
	~Terrain();

	bool init();
	void tick(float dt);
	void geometryPass(glm::mat4 projection, glm::mat4 view);
	void beginLightPasses();
	void directionalLightPass(glm::mat4 projection, glm::mat4 view);
	void render(glm::mat4 projection, glm::mat4 view);

	bool setTexture(GLenum TextureTarget, const char* fileName);

	void getDimensions(int& width, int& height) const;


private:

	// private functions
	bool generateMesh();
	void calculateNormal(float **data, int z, int x, VertexTN& vert);

	// private variables
	Engine *engine;
    const char* fileName;
	TerrainProgram program;
	DirectionalLightDSProgram dlProgram;
	Mesh *dirLightRenderQuad;
	std::vector<VertexTN> geometry;
	std::vector<unsigned int> indices;
	GLuint ibo;
	int width, height;
	float min, max, range;
	Texture* groundTexture;
	int heightScale;
	GBuffer buffer;

	GDALDataset* gdalDataSet;
};

} // end Vancom namepsace

#endif // END TERRAIN_HPP