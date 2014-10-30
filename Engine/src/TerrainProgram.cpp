#include "TerrainProgram.hpp"

#include <iostream>

using namespace Vancom;

TerrainProgram::TerrainProgram(){

}

bool TerrainProgram::init(){

    if (!ShaderProgram::init())
        return false;

    if (!addShader(GL_VERTEX_SHADER, "../shaders/terrain.vs"))
        return false;

	if (!addShader(GL_FRAGMENT_SHADER, "../shaders/terrain.fs"))
		return false;

	if (!finalize())
		return false;

    // get uniform and attribute positions in shader
    locMVP = getUniformLocation("mvp");
    locSampler = getUniformLocation("gSampler");
    locLightDir = getUniformLocation("lightDir");

    locPos = glGetAttribLocation(program, "vs_pos");
    locTex = glGetAttribLocation(program, "vs_tex");
    locNormal = glGetAttribLocation(program, "vs_normal");

    if(locMVP == -1 || locPos == -1 || locLightDir == -1 || locTex == -1 || locNormal == -1)
        std::cerr << "Unable to find location in TerrainProgram" << std::endl;

    return true;
}

void TerrainProgram::setMVP(const glm::mat4 MVP) const{

    glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(MVP));    
}

void TerrainProgram::setSampler(const int sampler) const{

    glUniform1i(locSampler, 0);    
}

void TerrainProgram::setLightDirection(const glm::vec3 direction) const{

    glUniform3fv(locLightDir, 1, glm::value_ptr(direction));
}