#ifndef EXPLOSION_HPP
#define	EXPLOSION_HPP

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier
#include "Texture.hpp"
#include "ExplosionUpdateProgram.hpp"
#include "ExplosionRenderProgram.hpp"
#include "RandomTexture.hpp"
#include "Particle.hpp"
#include "Entity.hpp"

// REFERENCE: ogldev.atspace.co.uk/www/tutorial22/tutorial28.html

namespace Vancom{

class Explosion : public Entity{

public:

	// public functions
	Explosion();
	~Explosion();
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
	ExplosionUpdateProgram updateProgram;
	ExplosionRenderProgram renderProgram;
	RandomTexture randomTexture;
	Texture* texture;
	long timeElapsed;
	float dt;
	glm::mat4 model;

};

} // end namespace Vancom

#endif	/* EXPLOSION_HPP */
