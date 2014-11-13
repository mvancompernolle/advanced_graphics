#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <GL/glew.h>
#include <GL/gl.h>

#include <string>
#include <SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "Model.hpp"
#include "SelectionProgram.hpp"
#include "SelectionTexture.hpp"
#include "TerrainProgram.hpp"
#include "SilhouetteProgram.hpp"
#include "GUIProgram.hpp"
#include "GeometryProgram.hpp"
#include "GBuffer.hpp"
#include "DirLightDSProgram.hpp"
#include "SpotLightDSProgram.hpp"
#include "PointLightDSProgram.hpp"

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


	// public variables
	glm::mat4 view, projection;
	Camera *camera;
	bool isRaining;

private:

	// private variables
	Engine* engine;
	SDL_Window *window;
	SDL_GLContext gl_context;
	glm::vec3 lightDir;

	// selection program
	SelectionProgram selectionProgram;
	SelectionTexture selectionTexture;

	// default program
	TerrainProgram defaultProgram;

	// silhouette program
	SilhouetteProgram silhouetteProgram;

	// gui program
	GUIProgram guiProgram;

	// defferred rendering
	GeometryProgram geometryProgram;
	DirLightDSProgram dirLightProgram;
	SpotLightDSProgram spotLightProgram;
	PointLightDSProgram pointLightProgram;
	Model *dirLightRenderQuad;
	GBuffer buffer;
	
};

} // end Vancom namespace

#endif // END GRAPHICS_HPP
