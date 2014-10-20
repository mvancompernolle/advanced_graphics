#ifndef SPARKLER_H
#define	SPARKLER_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier
#include "Texture.h"
#include "SparklerUpdateProgram.h"
#include "SparklerRenderProgram.h"
#include "RandomTexture.h"

// REFERENCE: ogldev.atspace.co.uk/www/tutorial22/tutorial28.html

struct Particle{
	float type;
	glm::vec3 pos;
	glm::vec3 vel;
	float lifeTimeMS;
};

class Sparkler{
public:
	Sparkler();
	~Sparkler();
	bool init(const glm::vec3& pos);
	void updateParticles(int dt);
	void renderParticles(glm::mat4 proj, glm::mat4 view, glm::vec3 camPos);
	void render(glm::mat4 proj, glm::mat4 view, glm::vec3 camPos, int dt);

private:
	bool firstTime;
	GLuint VBOIndex, TBOIndex;
	GLuint particleBuffer[2], transformFeedback[2];
	std::vector<float> vertices;
	SparklerUpdateProgram updateProgram;
	SparklerRenderProgram renderProgram;
	RandomTexture randomTexture;
	Texture* texture;
	long timeElapsed;
	glm::mat4 model;
};

#endif	/* SPARKLER_H */
