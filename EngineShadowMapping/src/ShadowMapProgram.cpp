#include "ShadowMapProgram.hpp"

// SOURCE : http://ogldev.atspace.co.uk/www/tutorial23/tutorial23.html

using namespace Vancom;

ShadowMapProgram::ShadowMapProgram(){
}

bool ShadowMapProgram::init(){

    if (!ShaderProgram::init())
        return false;

    if (!addShader(GL_VERTEX_SHADER, "../shaders/shadowMap.vs"))
        return false;

	if (!addShader(GL_FRAGMENT_SHADER, "../shaders/shadowMap.fs"))
		return false;

	if (!finalize())
		return false;

    // get uniform and attribute positions in shader
    locMVP = getUniformLocation("mvp");
    locTexture = getUniformLocation("gSampler");

    return true;
}

void ShadowMapProgram::setMVP(const glm::mat4 MVP){

    glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(MVP)); 
}

void ShadowMapProgram::setTextureUnit(unsigned int textureUnit){

	glUniform1i(locTexture, textureUnit);
}