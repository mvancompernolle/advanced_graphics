#include "LightningProgram.hpp"

using namespace Vancom;

LightningProgram::LightningProgram(){

}

bool LightningProgram::init(){
    
    if (!ShaderProgram::init())
        return false;

    if (!addShader(GL_VERTEX_SHADER, "../shaders/lightning.vs"))
        return false;

    if (!addShader(GL_GEOMETRY_SHADER, "../shaders/lightning.gs"))
        return false;

    if (!addShader(GL_FRAGMENT_SHADER, "../shaders/lightning.fs"))
        return false;

    if (!finalize())
        return false;

    locMVP = getUniformLocation("mvp");
    locPositions = getUniformLocation("positions");
    locRandomTexture = getUniformLocation("randomTexture");
    locTime = getUniformLocation("time");

    return true;
}

void LightningProgram::setMVP(const glm::mat4 MVP){

    glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(MVP));    
}

void LightningProgram::setPositions(const std::vector<glm::vec3> positions) const{

    glUniform3fv(locPositions, positions.size(), (float*) &positions[0]);
}

void LightningProgram::setRandomTextureUnit(unsigned int textureUnit) const{

    glUniform1i(locRandomTexture, textureUnit);
}

void LightningProgram::setTime(float time) const{

    glUniform1f(locTime, time);
}

