#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "ShaderProgram.hpp"

namespace Vancom{

class Entity{

public:

	// public functions
	Entity();
	~Entity();
	virtual void tick(float dt) = 0;
	virtual void render(glm::mat4 projection, glm::mat4 view) = 0;
	void scale(float scale);

protected:

	// protected functions
	virtual bool init() = 0;

	// protected variables
	GLuint vao, vbo;
	glm::mat4 model;

private:

};


} // end namespace Vancom

#endif // END ENTITY_HPP