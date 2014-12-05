#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Vertex.hpp"
#include "Entity.hpp"
#include "TerrainProgram.hpp"
#include "Mesh.hpp"

namespace Vancom{

class Engine;

class Model : public Entity{

public:
	Model(Engine *engine);
	Model(Engine *engine, glm::vec3 pos, float scale, float power, float intensity);
	~Model();
	bool init(const char* fileName);
	void tick(float dt);
	void render();
    btRigidBody* getRigidBody();

	// public variables
	bool moving;

private:

	// private variables
	Engine *engine;
	Mesh *mesh;
	float timeElapsed;
	float decisionTime;
	float speed, scale;
	glm::vec3 target, startingPos;
	btRigidBody* rigidBody;
};

}

#endif // END Model_HPP