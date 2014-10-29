#ifndef ENGINE_HPP
#define ENGINE_HPP

namespace Vancom{

class Graphics;
class Input;

class Engine {

public:

	// public functions
	Engine(int argc, char **argv);
	~Engine();

	void init();
	int run();

	void stop(int exit_code = 0);

	// public variables
	Graphics *graphics;
	Input *input;

private:

	// private variables
	int argc;
	char **argv;

};

} // closing namespace vancom

#endif // ENGINE_HPP