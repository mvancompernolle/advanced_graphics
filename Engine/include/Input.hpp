#ifndef INPUT_HPP
#define INPUT_HPP

#include <map>
#include <vector>

namespace Vancom{

class Engine;
class Entity;

class Input{

public:

	// public functions
	Input(Engine *engine);
	~Input();

	void init();
	void tick(float dt);
	void stop();

	// public variables
	std::vector<Entity*> selected;

private:

	// private functions
	void resetKeysPressed();
	void handleMovementKeys();

	// private variables
	Engine *engine;
	float sensitivity;
	std::map<int, bool> keysPressed;

};

}

#endif // END INPUT_HPP