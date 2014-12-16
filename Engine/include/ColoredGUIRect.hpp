#ifndef COLORED_GUI_RECT_HPP
#define COLORED_GUI_RECT_HPP

#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "Vertex.hpp"
#include "Entity.hpp"

namespace Vancom{

class Texture;
class Engine;

class ColoredGUIRect : public Entity{

public:

    // public functions
    ColoredGUIRect(Engine* engine, bool mainBar);
    ~ColoredGUIRect();
    bool init(glm::vec3 color, glm::vec2 topLeft, glm::vec2 bottomRight);
    void tick(float dt);
    void render();

    // public variables
    glm::vec3 color;

private:

    // private variables
    Engine* engine;
    std::vector<Vertex> box;
    bool mainBar;
    glm::vec2 topLeft, bottomRight;
};

}

#endif // END COLORED_GUI_RECT_HPP