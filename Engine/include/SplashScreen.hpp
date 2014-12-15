#ifndef SPLASH_SCREEN_HPP
#define SPLASH_SCREEN_HPP

#include <vector>
#include "Vertex.hpp"
#include "Entity.hpp"

namespace Vancom{

class Texture;

class SplashScreen : public Entity{

public:
    
    // private variables
    SplashScreen();
    ~SplashScreen();
    bool init(const char* fileName, float width, float height);
    void tick(float dt);
    void transitionOff();
    void render();

    // public variables
    float transVal;

private:

    // private variables
    std::vector<VertexT> box;
    Texture *texture;
    bool transitioning;
    float width, height;
};

}

#endif // END SPLASH_SCREEN_HPP