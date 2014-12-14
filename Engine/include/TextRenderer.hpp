#ifndef TEXT_RENDERER_HPP
#define TEXT_RENDERER_HPP

#include <ft2build.h>
#include <freetype.h>
#include "TextProgram.hpp"
#include "Entity.hpp"

namespace Vancom{

class TextRenderer : public Entity{

public:
    TextRenderer();
    ~TextRenderer();
    bool init();
    void tick(float dt);
    void render();
    void render(const char* text, float x, float y, float sx, float sy);

private:

    // private variables
    TextProgram program;
    FT_Library ftLibrary;
    FT_Face face;
    FT_GlyphSlot slot;
    GLuint textTexture;
};

}

#endif // END TEXT_RENDERER_HPP