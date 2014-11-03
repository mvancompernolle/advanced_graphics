#ifndef TERRAINBORDER_HPP
#define TERRAINBORDER_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "Entity.hpp"
#include "BorderUpdateProgram.hpp"
#include "BorderRenderProgram.hpp"

namespace Vancom{

// create stuct for border particle
struct Particle{
	float type;
	glm::vec3 pos;
	glm::vec3 vel;
	float lifeTimeMS;
};

class Engine;

class TerrainBorder : public Entity{

public:

	// public functions
	TerrainBorder(Engine *engine, float speed);
	~TerrainBorder();
	bool init(float renderDistance, glm::vec2 topLeft, glm::vec2 topRight, glm::vec2 bottomLeft, glm::vec2 BottomRight);
	void initParticles(Particle* particles);
	void tick(float dt);
	void updateParticles();
	void renderParticles(glm::mat4 proj, glm::mat4 view);
	void render(glm::mat4 projection, glm::mat4 view);

private:

	// private variables
	Engine *engine;
	bool isRendering, firstTime;
	GLuint VBOIndex, TBOIndex;
	GLuint particleBuffer[2], transformFeedback[2];	
	BorderRenderProgram renderProgram;
	BorderUpdateProgram updateProgram;
	glm::vec2 topLeft, bottomRight;
	float dt, speed, renderDistance, distToCamera;
	int particleCount;
};

} // end namespace Vancom

#endif // END TERRAINBORDER_HPP