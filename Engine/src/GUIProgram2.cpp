#include "GUIProgram2.hpp"

#include <iostream>

using namespace Vancom;

GUIProgram2::GUIProgram2(){

}

bool GUIProgram2::init(){

    if (!ShaderProgram::init())
        return false;

    if (!addShader(GL_VERTEX_SHADER, "../shaders/gui2.vs"))
        return false;

    if (!addShader(GL_FRAGMENT_SHADER, "../shaders/gui2.fs"))
        return false;

    if (!finalize())
        return false;

    // get uniform and attribute positions in shader
    locColor = getUniformLocation("color");

    return true;
}

void GUIProgram2::setColor(const glm::vec3 color) const{

    glUniform3fv(locColor, 1, glm::value_ptr(color));
}