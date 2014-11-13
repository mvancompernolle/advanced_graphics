#include "ExplosionRenderProgram.hpp"

using namespace Vancom;

ExplosionRenderProgram::ExplosionRenderProgram(){

}

bool ExplosionRenderProgram::init(){
    
    if (!ShaderProgram::init())
        return false;

    if (!addShader(GL_VERTEX_SHADER, "../shaders/explosionRender.vs"))
        return false;

    if (!addShader(GL_GEOMETRY_SHADER, "../shaders/explosionRender.gs"))
        return false;

	if (!addShader(GL_FRAGMENT_SHADER, "../shaders/explosionRender.fs"))
		return false;

	if (!finalize())
		return false;

    locMVP = getUniformLocation("mvp");
    locCameraPos = getUniformLocation("cameraPos");
    locColorMap = getUniformLocation("colorMap");
    locSize = getUniformLocation("size");

    return true;
}

void ExplosionRenderProgram::setMVP(const glm::mat4 MVP){
    glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(MVP));    
}

void ExplosionRenderProgram::setCameraPos(const glm::vec3& pos){
    glUniform3f(locCameraPos, pos.x, pos.y, pos.z);
}

void ExplosionRenderProgram::setColorTextureUnit(unsigned int textureUnit){
    glUniform1i(locColorMap, textureUnit);
}

void ExplosionRenderProgram::setBillboardSize(float billboardSize){
    glUniform1f(locSize, billboardSize);
}