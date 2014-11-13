#include "ExplosionUpdateProgram.hpp"

using namespace Vancom;

ExplosionUpdateProgram::ExplosionUpdateProgram(){

}

bool ExplosionUpdateProgram::init(){

    if (!ShaderProgram::init())
        return false;

    if (!addShader(GL_VERTEX_SHADER, "../shaders/explosionUpdate.vs"))
        return false;

    if (!addShader(GL_GEOMETRY_SHADER, "../shaders/explosionUpdate.gs"))
        return false;

	if (!finalize())
		return false;

	const GLchar* varyings[4];
    varyings[0] = "type1";
    varyings[1] = "pos1";
    varyings[2] = "vel1";    
    varyings[3] = "age1";

    glTransformFeedbackVaryings(program, 4, varyings, GL_INTERLEAVED_ATTRIBS);

    if (!finalize()) {
        return false;
    }

    locDt = getUniformLocation("dt");
    locRandomTexture = getUniformLocation("randomTexture");
    locTime = getUniformLocation("time");
    locLauncherLifeTime = getUniformLocation("launcherLifeTime");
    locShellLifeTime = getUniformLocation("shellLifeTime");
    locSecShellLifeTime = getUniformLocation("secShellLifeTime");

    return true;
}

void ExplosionUpdateProgram::setDt(unsigned int dt){
    glUniform1f(locDt, (float)dt);
}

void ExplosionUpdateProgram::setTime(int time){
    glUniform1f(locTime, (float)time);
}

void ExplosionUpdateProgram::setRandomTextureUnit(unsigned int textureUnit){
    glUniform1i(locRandomTexture, textureUnit);
}

void ExplosionUpdateProgram::setLauncherLifetime(float lifetime){
    glUniform1f(locLauncherLifeTime, lifetime);
}

void ExplosionUpdateProgram::setShellLifetime(float lifetime){
    glUniform1f(locShellLifeTime, lifetime);
}

void ExplosionUpdateProgram::setSecondaryShellLifetime(float lifetime){
    glUniform1f(locSecShellLifeTime, lifetime);
}