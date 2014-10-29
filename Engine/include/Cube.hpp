#ifndef CUBE_HPP
#define CUBE_HPP

#include <vector>
#include "Vertex.hpp"
#include "Entity.hpp"
#include "DefaultProgram.hpp"

namespace Vancom{

class DefaultProgram;

class Cube : public Entity{

public:
	Cube();
	~Cube();
	void tick(float dt);
	void render(glm::mat4 projection, glm::mat4 view);

protected:

	bool init();

private:

	// private variables
	DefaultProgram program;
	std::vector<VertexC> geometry;

};

}

#endif // END CUBE_HPP