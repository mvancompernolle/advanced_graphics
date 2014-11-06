#include "DirectionalLightDSProgram.hpp"

#include <iostream>

using namespace Vancom;

DirectionalLightDSProgram::DirectionalLightDSProgram(){

}

bool DirectionalLightDSProgram::init(){

    if (!ShaderProgram::init())
        return false;

    if (!addShader(GL_VERTEX_SHADER, "../shaders/light.vs"))
        return false;

    if (!addShader(GL_FRAGMENT_SHADER, "../shaders/dirLight.fs"))
        return false;

    if (!finalize())
        return false;

    // get uniforms
    locLightDir = getUniformLocation("lightDir");

    return LightDSProgram::init();
}

void DirectionalLightDSProgram::setLightDirection(const glm::vec3 direction) const{

    glUniform3fv(locLightDir, 1, glm::value_ptr(direction));
}