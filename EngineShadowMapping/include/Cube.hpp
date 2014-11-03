#ifndef CUBE_HPP
#define CUBE_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <vector>
#include "Vertex.hpp"
#include "Entity.hpp"
#include "ShadowMapProgram.hpp"

namespace Vancom{

class DefaultProgram;

class Cube : public Entity{

public:
	Cube(glm::vec3 pos);
	~Cube();
	bool init(float size);
	void tick(float dt);
	void render(glm::mat4 projection, glm::mat4 view);
	void render(glm::mat4 projection, glm::mat4 view, ShadowMapProgram program);
private:

	// private variables
	std::vector<VertexTN> geometry;

};

}

#endif // END CUBE_HPP