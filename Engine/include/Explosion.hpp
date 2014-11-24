#ifndef EXPLOSION_HPP
#define	EXPLOSION_HPP

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier
#include "Texture.hpp"
#include "ExplosionUpdateProgram.hpp"
#include "FireworksRenderProgram.hpp"
#include "Particle.hpp"
#include "Entity.hpp"

// REFERENCE: ogldev.atspace.co.uk/www/tutorial22/tutorial28.html

namespace Vancom{

class EntityManager;

class Explosion : public Entity{

public:

	// public functions
	Explosion(EntityManager *mgr);
	~Explosion();
	bool init(const glm::vec3& pos);
	void tick(float dt);
	void updateParticles();
	void renderParticles(glm::mat4 proj, glm::mat4 view, glm::vec3 camPos);
	void render();
	void render(glm::mat4 proj, glm::mat4 view, glm::vec3 camPos);

private:

	// private variables
	EntityManager *mgr;
	bool firstTime;
	GLuint VBOIndex, TBOIndex;
	GLuint particleBuffer[2], transformFeedback[2];
	std::vector<float> vertices;
	ExplosionUpdateProgram updateProgram;
	FireworksRenderProgram renderProgram;
	Texture* texture;
	long timeElapsed;
	float dt;

};

} // end namespace Vancom

#endif	/* EXPLOSION_HPP */
