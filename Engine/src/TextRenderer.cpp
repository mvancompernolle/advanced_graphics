#include "TextRenderer.hpp"

#include <iostream>

using namespace Vancom;

TextRenderer::TextRenderer(){

}

TextRenderer::~TextRenderer(){
    FT_Done_Face(face);
    FT_Done_FreeType(ftLibrary);
}

bool TextRenderer::init(){

    // init program
    if(!program.init()){
        return false;
    }

    program.enable();
    program.setSampler(0);
    program.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

    // init freetype
    FT_Error error = FT_Init_FreeType(&ftLibrary);
    if(error){
        std::cout << "Freetype failed to init." << std::endl;
    }

    error = FT_New_Face(ftLibrary, "../assets/FreeSans.ttf", 0, &face);
    if(error == FT_Err_Unknown_File_Format){
        std::cout << "Font format is not supported." << std::endl;
    }
    else if(error){
        std::cout << "Font file could not be opened." << std::endl;        
    }

    error = FT_Set_Pixel_Sizes(face, 0, 48);
    slot = face->glyph;

    // create vao
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // create vbo
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // setup attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0); // pos

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &textTexture);
    glBindTexture(GL_TEXTURE_2D, textTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    return true;
}

void TextRenderer::tick(float dt){

}

void TextRenderer::render(){

}

void TextRenderer::render(const char* text, float x, float y, float sx, float sy){

    program.enable();

    // bind vao
    glBindVertexArray(vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textTexture);

    const char *p;

    for(p = text; *p; p++) {
        if(FT_Load_Char(face, *p, FT_LOAD_RENDER))
            continue;

        glTexImage2D(
          GL_TEXTURE_2D,
          0,
          GL_ALPHA,
          slot->bitmap.width,
          slot->bitmap.rows,
          0,
          GL_ALPHA,
          GL_UNSIGNED_BYTE,
          slot->bitmap.buffer
        );

        float x2 = x + slot->bitmap_left * sx;
        float y2 = -y - slot->bitmap_top * sy;
        float w = slot->bitmap.width * sx;
        float h = slot->bitmap.rows * sy;

        std::cout << x2 << " " << -y2 << " " << w << " " << -h << " " << " " << *p << std::endl;

        GLfloat box[4][4] = {
            {0,0   , 0, 0},
            {0,1     , 1, 0},
            {1,0 , 0, 1},
            {1,1 , 1, 1},
        };

        glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        x += (slot->advance.x >> 6) * sx;
        y += (slot->advance.y >> 6) * sy;
    }
}