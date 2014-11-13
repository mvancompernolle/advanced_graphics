#ifndef PARTICLE_HPP
#define PARTICLE_HPP

struct Particle{
	float type;
	glm::vec3 pos;
	glm::vec3 vel;
	float lifeTimeMS;
};

#endif // end PARTICLE_HPP