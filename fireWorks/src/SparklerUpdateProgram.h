#ifndef SPARKLER_UPDATE_PROGRAM_H
#define	SPARKLER_UPDATE_PROGRAM_H

#include "RenderProgram.h"

class SparklerUpdateProgram : public RenderProgram
{
public:
    SparklerUpdateProgram();
    virtual bool init();    
    void setDt(unsigned int dt);
    void setTime(int time);
    void setRandomTextureUnit(unsigned int textureUnit);
    void setLauncherLifetime(float lifetime);
    void setShellLifetime(float lifetime);
    void setSecondaryShellLifetime(float lifetime);
    
 private:
    GLuint locDt;
    GLuint locRandomTexture;
    GLuint locTime;
    GLuint locLauncherLifeTime;
    GLuint locShellLifeTime;
    GLuint locSecShellLifeTime;
};


#endif	/* SPARKLER_UPDATE_PROGRAM_H */