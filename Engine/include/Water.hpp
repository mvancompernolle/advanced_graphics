#ifndef WATER_HPP
#define WATER_HPP

#include <glm/glm.hpp>

#include "Entity.hpp"
#include "WaterProgram.hpp"
#include "Vertex.hpp"
#include <vector>

namespace Vancom{

class Engine;
class Texture;

class Water : public Entity{

public:

    // public functions
    Water(Engine *engine, float w, float h);
    ~Water();

    bool init();
    void tick(float dt);
    void render();
    void render(glm::mat4 projection, glm::mat4 view);
    bool setTexture(GLenum TextureTarget, const char* fileName);

private:

    // private variables
    Engine *engine;
    float width, height, density, time;
    WaterProgram program;
    std::vector<VertexT> geometry;
    std::vector<unsigned int> indices;
    Texture* waterTexture;
    GLuint ibo;

};

} // end namespace Vancom

#endif // WATER_HPP