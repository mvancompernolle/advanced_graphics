#ifndef GRASS_HPP
#define	GRASS_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier
#include <vector>
#include <random>

#include "Vertex.hpp"
#include "Entity.hpp"
#include "GrassProgram.hpp"

namespace Vancom{

class Engine;

class Grass : public Entity{

public:
    // public functions
    Grass(Engine *engine, glm::mat4 model);
    ~Grass();
    void generateBlades(const std::vector<VertexTN>& verts);

    bool init();
    void tick(float dt);
    void render();
    void render(glm::mat4 projection, glm::mat4 view);

private:
    // private functions

    // private variables
    Engine *engine;
    GrassProgram program;
    std::vector<Vertex> blades;
};

}

#endif	/* GRASS_HPP */

