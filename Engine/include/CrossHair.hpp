#ifndef CROSS_HAIR_HPP
#define CROSS_HAIR_HPP

#include <vector>
#include "Vertex.hpp"
#include "Entity.hpp"

namespace Vancom{

class Texture;

class CrossHair : public Entity{

public:
	CrossHair();
	~CrossHair();
	bool init(const char* fileName, float width, float height);
	void tick(float dt);
	void render();

private:

	// private variables
	std::vector<VertexT> box;
	Texture *texture;
};

}

#endif // END CROSS_HAIR_HPP