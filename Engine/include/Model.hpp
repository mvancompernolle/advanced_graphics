#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include "Vertex.hpp"
#include "Entity.hpp"
#include "TerrainProgram.hpp"
#include "Mesh.hpp"

namespace Vancom{

class DefaultProgram;

class Model : public Entity{

public:
	Model();
	Model(glm::vec3 pos, float scale);
	~Model();
	bool init(const char* fileName);
	void tick(float dt);
	void render();

private:

	// private variables
	Mesh *mesh;
};

}

#endif // END Model_HPP