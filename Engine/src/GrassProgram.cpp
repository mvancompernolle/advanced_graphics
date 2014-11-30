#include "GrassProgram.hpp"

#include <iostream>

using namespace Vancom;

GrassProgram::GrassProgram(){

}

bool GrassProgram::init(){

    if (!ShaderProgram::init())
        return false;

    if (!addShader(GL_VERTEX_SHADER, "../shaders/grass.vs"))
        return false;

    if (!addShader(GL_GEOMETRY_SHADER, "../shaders/grass.gs"))
        return false;

	if (!addShader(GL_FRAGMENT_SHADER, "../shaders/grass.fs"))
		return false;

	if (!finalize())
		return false;

	// get uniform locations
    locMVP = getUniformLocation("mvp");
    locWindDir = getUniformLocation("windDir");

    return true;
}

void GrassProgram::setMVP(const glm::mat4 MVP){

    glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(MVP));    
}

void GrassProgram::setWindDir(const glm::vec3 windDir){

    glUniform3fv(locWindDir, 1, glm::value_ptr(windDir));
}