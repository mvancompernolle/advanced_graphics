#ifndef MESH_HPP
#define	MESH_HPP

#include <vector>
#include <GL/glew.h>
#include <assimp/Importer.hpp>     
#include <assimp/scene.h>          
#include <assimp/postprocess.h>
#include "Texture.hpp"
#include <iostream>
#include "Vertex.hpp"
#include "TerrainProgram.hpp"

// REFERENCE: ogldev.atspace.co.uk/www/tutorial22/tutorial22.html

namespace Vancom{

class Mesh{

#define INVALID_MATERIAL 0xFFFFFFFF
   
#define INDEX_BUFFER 0    
#define POS_VB       1
#define TEXCOORD_VB  2
#define NORMAL_VB    3  	

public:

	// public functions
	Mesh(TerrainProgram program);
	bool loadMesh(const char* fileName);
	void renderMesh();

private:

	// private functions
	bool loadMeshesFromScene(const aiScene* scene, const char* fileName);
	void initializeMesh(const aiMesh* mesh, std::vector<glm::vec3>& positions, 
		std::vector<glm::vec3>& normals, std::vector<glm::vec2>& texCoords, std::vector<unsigned int>& indices);
	bool initializeMaterials(const aiScene* scene, const char* fileName);
	
	struct MeshUnit{

        MeshUnit();

		unsigned int numIndices;
		unsigned int materialIndex;
		unsigned int baseIndex;
		unsigned int baseVertex;

	};

	// private variables
	GLuint vao;
	GLuint buffers[4];
	std::vector<MeshUnit> meshes;
	std::vector<Texture*> textures;
	TerrainProgram program;
};

}	// end namespace Vancom

#endif	// MESH_HPP 