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
    locColor = getUniformLocation("dirLight.base.color");
    locAmbient = getUniformLocation("dirLight.base.ambientIntensity");
    locDiffuse = getUniformLocation("dirLight.base.diffuseIntensity");
    locDir = getUniformLocation("dirLight.direction");
    locCameraPos = getUniformLocation("cameraPos");

    return true;
}

void WaterProgram::setMVP(const glm::mat4 MVP) const{

    glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(MVP));    
}

void WaterProgram::setCameraPosition(const glm::vec3 pos) const{

    glUniform3fv(locCameraPos, 1, glm::value_ptr(pos));
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

void WaterProgram::setDirLight(const DirectionalLight& light) const{

    glUniform3fv(locColor, 1, glm::value_ptr(light.color));
    glUniform1f(locAmbient, light.ambientIntensity);
    glUniform1f(locDiffuse, light.diffuseIntensity);
    glUniform3fv(locDir, 1, glm::value_ptr(light.direction));
}
