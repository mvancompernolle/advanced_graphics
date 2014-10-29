#ifndef INPUT_HPP
#define INPUT_HPP

#include <map>

namespace Vancom{

class Engine;

class Input{

public:

	// public functions
	Input(Engine *engine);
	~Input();

	void init();
	void tick(float dt);
	void stop();

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