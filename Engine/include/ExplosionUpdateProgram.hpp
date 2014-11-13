#ifndef EXPLOSION_UPDATE_PROGRAM_HPP
#define	EXPLOSION_UPDATE_PROGRAM_HPP

#include "ShaderProgram.hpp"

namespace Vancom{

class ExplosionUpdateProgram : public ShaderProgram{

public:

    // public functions
    ExplosionUpdateProgram();
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


#endif	/* EXPLOSION_UPDATE_PROGRAM_HPP */