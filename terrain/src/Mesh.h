#ifndef MESH_H
#define	MESH_H

#include <vector>
#include <GL/glew.h>
#include <assimp/Importer.hpp>     
#include <assimp/scene.h>          
#include <assimp/postprocess.h>
#include "Texture.h"
#include <sstream>
#include <string>
#include <iostream>
#include "Vertex.h"
#include <btBulletDynamicsCommon.h>

// REFERENCE: ogldev.atspace.co.uk/www/tutorial22/tutorial22.html

class Mesh{
public:
	Mesh();
	bool loadMesh(const char* fileName);
	void renderMesh(GLint, GLint);
	btCollisionShape* getCollisionShape();

private:
	bool loadMeshesFromScene(const aiScene* scene, const char* fileName);
	void initializeMesh(unsigned int index, const aiMesh* mesh);
	bool initializeMaterials(const aiScene* scene, const char* fileName);
	btVector3 convertFArrayTobtVector(float arr[]);
	void clear();
	
	struct MeshUnit{
        MeshUnit();
		void initialize(const std::vector<Vertex>& Vertices);

		GLuint VBO;
		unsigned int numFaces;
		unsigned int materialIndex;
	};

	std::vector<MeshUnit> meshes;
	std::vector<Texture*> textures;
	btTriangleMesh* trimesh;
	btCollisionShape* shape;
};

#endif	/* MESH_H */
