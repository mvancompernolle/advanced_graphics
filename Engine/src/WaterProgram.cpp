#include "WaterProgram.hpp"

#include <iostream>

using namespace Vancom;

WaterProgram::WaterProgram(){

}

bool WaterProgram::init(){

    if (!ShaderProgram::init())
        return false;

    if (!addShader(GL_VERTEX_SHADER, "../shaders/water.vs"))
        return false;

    if (!addShader(GL_FRAGMENT_SHADER, "../shaders/water.fs"))
        return false;

    if (!finalize())
        return false;

    // get uniform locations
    locMVP = getUniformLocation("mvp");
    locModel = getUniformLocation("model");
    locTime = getUniformLocation("time");
    locTextureUnit = getUniformLocation("textureUnit");

    return true;
}

void WaterProgram::setMVP(const glm::mat4 MVP) const{

    glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(MVP));    
}

void WaterProgram::setModel(const glm::mat4 model) const{ 

    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));    
}

void WaterProgram::setTextureUnit(unsigned int textureUnit) const{

    glUniform1i(locTextureUnit, textureUnit);
}

void WaterProgram::setTime(float time) const{

    glUniform1f(locTime, time);
}