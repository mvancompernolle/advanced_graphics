#include "TextProgram.hpp"

#include <iostream>

using namespace Vancom;

TextProgram::TextProgram(){

}

bool TextProgram::init(){

    if (!ShaderProgram::init())
        return false;

    if (!addShader(GL_VERTEX_SHADER, "../shaders/text.vs"))
        return false;

    if (!addShader(GL_FRAGMENT_SHADER, "../shaders/text.fs"))
        return false;

    if (!finalize())
        return false;

    // get uniform and attribute positions in shader
    locSampler = getUniformLocation("tex");
    locColor = getUniformLocation("color");

    return true;
}

void TextProgram::setSampler(const int sampler) const{

    glUniform1i(locSampler, 0);    
}

void TextProgram::setColor(glm::vec4 color) const{
    glUniform4fv(locColor, 1, glm::value_ptr(color));
}