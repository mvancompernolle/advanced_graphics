#include "SkyBoxProgram.hpp"

// SOURCE : http://ogldev.atspace.co.uk/www/tutorial25/tutorial25.html

#include <iostream>

using namespace Vancom;

SkyBoxProgram::SkyBoxProgram(){

}

bool SkyBoxProgram::init()
{

    if (!ShaderProgram::init()) {
        return false;
    }

    if (!addShader(GL_VERTEX_SHADER, "../shaders/skybox.vs")) {
        return false;
    }

    if (!addShader(GL_FRAGMENT_SHADER, "../shaders/skybox.fs")) {
        return false;
    }

    if (!finalize()) {
        return false;
    }

    locMVP = getUniformLocation("mvp");
    locTexture = getUniformLocation("textureUnit");
    locLightDir = getUniformLocation("lightDir");

    return true;
}

void SkyBoxProgram::setMVP(const glm::mat4 mvp) const{

    glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(mvp));   
}

void SkyBoxProgram::setTextureUnit(unsigned int textureUnit) const{

    glUniform1i(locTexture, textureUnit);    
}

void SkyBoxProgram::setLightDir(const glm::vec3& dir) const{

    glUniform3fv(locLightDir, 1, glm::value_ptr(dir));
}