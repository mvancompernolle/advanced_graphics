#include "SparklerRenderProgram.h"

SparklerRenderProgram::SparklerRenderProgram(){

}

bool SparklerRenderProgram::init(){
    if (!RenderProgram::init())
        return false;

    if (!addShader(GL_VERTEX_SHADER, "shaders/sparklerRender.vs"))
        return false;

    if (!addShader(GL_GEOMETRY_SHADER, "shaders/sparklerRender.gs"))
        return false;

	if (!addShader(GL_FRAGMENT_SHADER, "shaders/sparklerRender.fs"))
		return false;

	if (!finalize())
		return false;

    locMVP = getUniformLocation("mvp");
    locCameraPos = getUniformLocation("cameraPos");
    locColorMap = getUniformLocation("colorMap");
    locSize = getUniformLocation("size");

    return true;
}

void SparklerRenderProgram::setMVP(const glm::mat4 MVP){
    glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(MVP));    
}

void SparklerRenderProgram::setCameraPos(const glm::vec3& pos){
    glUniform3f(locCameraPos, pos.x, pos.y, pos.z);
}

void SparklerRenderProgram::setColorTextureUnit(unsigned int textureUnit){
    glUniform1i(locColorMap, textureUnit);
}

void SparklerRenderProgram::setBillboardSize(float billboardSize){
    glUniform1f(locSize, billboardSize);
}

void SparklerRenderProgram::setView(const glm::mat4 view){
   // glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(view));    
}