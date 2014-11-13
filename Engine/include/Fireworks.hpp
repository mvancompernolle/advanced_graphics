#ifndef FIREWORKS_HPP
#define	FIREWORKS_HPP

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier
#include "Texture.hpp"
#include "FireworksUpdateProgram.hpp"
#include "FireworksRenderProgram.hpp"
#include "RandomTexture.hpp"
#include "Particle.hpp"
#include "Entity.hpp"

// REFERENCE: ogldev.atspace.co.uk/www/tutorial22/tutorial28.html

namespace Vancom{

class Fireworks : public Entity{

public:

	// public functions
	Fireworks();
	~Fireworks();
	bool init(const glm::vec3& pos);
	void tick(float dt);
	void updateParticles();
	void renderParticles(glm::mat4 proj, glm::mat4 view, glm::vec3 camPos);
	void render();
	void render(glm::mat4 proj, glm::mat4 view, glm::vec3 camPos);

private:

	// public variables
	bool firstTime;
	GLuint VBOIndex, TBOIndex;
	GLuint particleBuffer[2], transformFeedback[2];
	std::vector<float> vertices;
	FireworksUpdateProgram updateProgram;
	FireworksRenderProgram renderProgram;
	RandomTexture randomTexture;
	Texture* texture;
	long timeElapsed;
	float dt;
	glm::mat4 model;

};

} // end namespace Vancom

#endif	/* FIREWORKS_HPP */
