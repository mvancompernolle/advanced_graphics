#include <GL/glew.h> // glew must be included before the main gl libs
#include <GL/freeglut.h> // doing otherwise causes compiler shouting
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <glm/glm.hpp>
#include <FreeImagePlus.h>


#ifdef ASSIMP_3
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#else
#include <assimp/assimp.hpp>
#include <assimp/aiScene.h>
#include <assimp/aiPostProcess.h>
#endif
struct Vertex
{
    GLfloat position[3];
    GLfloat color[3];
};


class ModelLoader
{
	public:
		ModelLoader();
		std::vector<Vertex> loadObj(const char * obj, int &size);
		void loadPGMTerrain(const char * obj);




};
