#include "LightDSProgram.hpp"

// SOURCE : http://ogldev.atspace.co.uk/www/tutorial36/tutorial36.html

#include <iostream>

using namespace Vancom;

LightDSProgram::LightDSProgram(){

}

bool LightDSProgram::init(){

    // get uniforms
    locMVP = getUniformLocation("mvp");
    locPosTexture = getUniformLocation("gPositionMap");
    locColorTexture = getUniformLocation("gColorMap");
    locNormalTexture = getUniformLocation("gNormalMap");
    locScreenSize = getUniformLocation("screenSize");


    return true;
}

void LightDSProgram::setMVP(const glm::mat4 MVP) const{

    glUniformMatrix4fv(locMVP, 1, GL_FALSE, glm::value_ptr(MVP));    
}

void LightDSProgram::setPositionTextureUnit(unsigned int textureUnit){
std::cout << locPosTexture << " " << textureUnit << std::endl;
    glUniform1i(locPosTexture, textureUnit);
}

void LightDSProgram::setColorTextureUnit(unsigned int textureUnit){
    std::cout << locColorTexture << " " << textureUnit << std::endl;
    glUniform1i(locColorTexture, textureUnit);
}

void LightDSProgram::setNormalTextureUnit(unsigned int textureUnit){
std::cout << locNormalTexture << " " << textureUnit << std::endl;
    glUniform1i(locNormalTexture, textureUnit);
}

void LightDSProgram::setScreenSize(unsigned int width, unsigned int height){
std::cout << locScreenSize << " " << width << " " << height << std::endl;
    glUniform2f(locScreenSize, (float) width, (float) height);
}
