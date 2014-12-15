#include "GUIProgram.hpp"

#include <iostream>

using namespace Vancom;

GUIProgram::GUIProgram(){

}

bool GUIProgram::init(){

    if (!ShaderProgram::init())
        return false;

    if (!addShader(GL_VERTEX_SHADER, "../shaders/gui.vs"))
        return false;

	if (!addShader(GL_FRAGMENT_SHADER, "../shaders/gui.fs"))
		return false;

	if (!finalize())
		return false;

    // get uniform and attribute positions in shader
    locSampler = getUniformLocation("textureSampler");
    locDropColor = getUniformLocation("dropColor");
    locDropEnabled = getUniformLocation("dropEnabled");

    return true;
}

void GUIProgram::setSampler(const int sampler) const{

    glUniform1i(locSampler, 0);    
}

void GUIProgram::setDropEnabled(const int val) const{

    glUniform1i(locDropEnabled, val); 
}

void GUIProgram::setDropColor(const glm::vec3 color) const{

    glUniform3fv(locDropColor, 1, glm::value_ptr(color));
}