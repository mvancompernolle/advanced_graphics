#ifndef LIGHTING_MANAGER_HPP
#define LIGHTING_MANAGER_HPP

#include <vector>
#include "Lights.hpp"

namespace Vancom{

class Engine;

class LightingManager {

public:

	// public functions
	LightingManager(Engine *engine);
	~LightingManager();

	void init();
	void tick(float dt);
	void stop();

	void setDirectionalLight(DirectionalLight dirLight);
	void addSpotLight(SpotLight spotLight);
	void addPointLight(PointLight pointLight);
	void increaseLightAngle();
	void decreaseLightAngle();

	// public variables
	DirectionalLight dirLight;
	std::vector<SpotLight> spotLights;
	std::vector<PointLight> pointLights;

private:

	// private variables
	Engine *engine;
	float dirLightAngle;

};

} // end namespace Vancom

#endif // END LIGHTING_MANAGER_HPP