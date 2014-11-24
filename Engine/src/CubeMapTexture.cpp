#include "CubeMapTexture.hpp"

#include <FreeImagePlus.h>
#include <iostream>

// SOURCE : http://ogldev.atspace.co.uk/www/tutorial25/tutorial25.html

using namespace Vancom;

static const GLenum types[6] = {  GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                                  GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                                  GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                                  GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                                  GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                                  GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

CubeMapTexture::CubeMapTexture(const std::string& directory,
    const std::string& posXFilename,
    const std::string& negXFilename,
    const std::string& posYFilename,
    const std::string& negYFilename,
    const std::string& posZFilename,
    const std::string& negZFilename){

    std::string::const_iterator it = directory.end();
    it--;
    std::string baseDir = (*it == '/') ? directory : directory + "/";
    
    fileNames[0] = baseDir + posXFilename;
    fileNames[1] = baseDir + negXFilename;
    fileNames[2] = baseDir + posYFilename;
    fileNames[3] = baseDir + negYFilename;
    fileNames[4] = baseDir + posZFilename;
    fileNames[5] = baseDir + negZFilename;
    
    texture = 0;
}

CubeMapTexture::~CubeMapTexture(){

    if (texture != 0) {
        glDeleteTextures(1, &texture);
    }
}

bool CubeMapTexture::load(){

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    fipImage image;

    for (unsigned int i = 0; i < 6; i++) {
        // if texture found, load it
        if(image.load(fileNames[i].c_str())) {

            // if unknown image type, return
            if(image.getImageType() == FIT_UNKNOWN) {
                std::cerr << "Unkown image type!" << std::endl;
                return false;
            }

            // convert image to 32 bit pixels
            image.convertTo32Bits();
            image.flipVertical();
            //image.flipHorizontal();
        }
        else{
            return false;
        }

        // load OpenGL texture
        glTexImage2D(types[i], 0, GL_RGBA, image.getWidth(), image.getHeight(),
            0, GL_BGRA, GL_UNSIGNED_BYTE, (void*) image.accessPixels());
        
    }    
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);           
    
    return true;
}

void CubeMapTexture::bind(GLenum textureUnit){

    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
}