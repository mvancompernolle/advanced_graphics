#ifndef FIREWORKS_UPDATE_PROGRAM_HPP
#define	FIREWORKS_UPDATE_PROGRAM_HPP

#include "ShaderProgram.hpp"

namespace Vancom{

class FireworksUpdateProgram : public ShaderProgram{

public:

    // public functions
    FireworksUpdateProgram();
    virtual bool init();    
    void setDt(unsigned int dt);
    void setTime(int time);
    void setRandomTextureUnit(unsigned int textureUnit);
    void setLauncherLifetime(float lifetime);
    void setShellLifetime(float lifetime);
    void setSecondaryShellLifetime(float lifetime);
    
 private:

    // private variables
    GLuint locDt;
    GLuint locRandomTexture;
    GLuint locTime;
    GLuint locLauncherLifeTime;
    GLuint locShellLifeTime;
    GLuint locSecShellLifeTime;
};

}


#endif	/* FIREWORKS_UPDATE_PROGRAM_HPP */