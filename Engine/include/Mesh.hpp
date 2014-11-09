#ifndef MESH_HPP
#define	MESH_HPP

#include <vector>
#include <map>
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

struct Edge{

    Edge(unsigned int _a, unsigned int _b)
    {
        //assert(_a != _b);
        
        if (_a < _b){
            a = _a;
            b = _b;                   
        }
        else{
            a = _b;
            b = _a;
        }
    }

    void print()
    {
        printf("Edge %d %d\n", a, b);
    }
    
    unsigned int a;
    unsigned int b;
};

struct Neighbors{

    unsigned int n1;
    unsigned int n2;
    
    Neighbors(){
        n1 = n2 = (unsigned int)-1;
    }
    
    void addNeighbor(unsigned int n){
        if (n1 == -1) {
            n1 = n;
        }
        else if (n2 == -1) {
            n2 = n;
        }
        else {
            //assert(0);
        }
    }
    
    unsigned int getOther(unsigned int me) const{
        if (n1 == me) {
            return n2;
        }
        else if (n2 == me) {
            return n1;
        }
        else {
            //assert(0);
        }

        return 0;
    }
};

struct CompareEdges{

    bool operator()(const Edge& edge1, const Edge& edge2)
    {
        if (edge1.a < edge2.a) {
            return true;
        }
        else if (edge1.a == edge2.a) {
            return (edge1.b < edge2.b);
        }        
        else {
            return false;
        }            
    }
};


struct CompareVectors{

    bool operator()(const aiVector3D& a, const aiVector3D& b){

        if (a.x < b.x) {
            return true;
        }
        else if (a.x == b.x) {
            if (a.y < b.y) {
                return true;
            }
            else if (a.y == b.y) {
                if (a.z < b.z) {
                    return true;
                }
            }
        }
        
        return false;
    }
};

struct Face{

    unsigned int indices[3];
    
    unsigned int getOppositeIndex(const Edge& e) const{

        for (unsigned int i = 0 ; i < 3; i++) {
            unsigned index = indices[i];
            
            if (index != e.a && index != e.b) {
                return index;
            }
        }

        return 0;
    }
};

class Mesh{

#define INVALID_MATERIAL 0xFFFFFFFF
   
#define INDEX_BUFFER 0    
#define POS_VB       1
#define TEXCOORD_VB  2
#define NORMAL_VB    3  	

public:

	// public functions
	Mesh();
	bool loadMesh(const char* fileName, bool withAdjacencies);
	void renderMesh();

private:

	// private functions
	bool loadMeshesFromScene(const aiScene* scene, const char* fileName);
	void initializeMesh(const aiMesh* mesh, std::vector<glm::vec3>& positions, 
		std::vector<glm::vec3>& normals, std::vector<glm::vec2>& texCoords, std::vector<unsigned int>& indices);
	bool initializeMaterials(const aiScene* scene, const char* fileName);
	void findAdjacencies(const aiMesh* mesh, std::vector<unsigned int>& indices);
	
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
	bool withAdjacencies;
	std::vector<MeshUnit> meshes;
	std::vector<Texture*> textures;
    std::map<Edge, Neighbors, CompareEdges> indexMap;
	std::map<aiVector3D, unsigned int, CompareVectors> posMap;
    std::vector<Face> uniqueFaces;
};

}	// end namespace Vancom

#endif	// MESH_HPP 