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
	virtual ~Entity();
	virtual void tick(float dt) = 0;
	virtual void render() = 0;
	void scale(float scale);

	glm::mat4 getModel() const;
	glm::vec3 getPos() const;

	// public variables
	unsigned int id;
	float specularIntensity, specularPower;
	bool updating;

protected:
	
	// protected variables
	GLuint vao, vbo;
	glm::mat4 model;

private:

};


} // end namespace Vancom

#endif // END ENTITY_HPP