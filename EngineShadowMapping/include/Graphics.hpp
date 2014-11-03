#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <GL/glew.h>
#include <GL/gl.h>

#include <string>
#include <SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "Cube.hpp"
#include "ShadowMapProgram.hpp"
#include "ShadowMapFBO.hpp"
#include "Texture.hpp"

namespace Vancom{

// declare classes to decrease compilation time
class Engine;
class Camera;

class Graphics{

public:

	// public functions
	Graphics(Engine *engine);
	~Graphics();

	void init();
	void tick(float dt);
	void render();
	void stop();

	void updateView();
	void updateCamera();
	void windowResized();

	void getWindowSize(int &w, int &h) const;
	void setClearColor(glm::vec3);

	void increaseLightAngle();
	void decreaseLightAngle();
	glm::vec3 getLightDirection() const;

	// public variables
	glm::mat4 view, projection;
	Camera *camera;

private:

	// private variables
	Engine* engine;
	SDL_Window *window;
	SDL_GLContext gl_context;
	float lightAngle;
	glm::vec3 lightDir;

	// temp variable for shadow rendering
	ShadowMapProgram shadowProgram;
	ShadowMapFBO shadowFBO;
	Cube *cube1, *cube2;
	GLuint shadowVao;
	Texture* tex;
};

} // end Vancom namespace

#endif // END GRAPHICS_HPP
