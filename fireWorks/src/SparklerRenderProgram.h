#ifndef SPARKLER_RENDER_PROGRAM_H
#define	SPARKLER_RENDER_PROGRAM_H

#include "RenderProgram.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

class SparklerRenderProgram : public RenderProgram
{
public:
    SparklerRenderProgram();
    virtual bool init();    
    void setMVP(const glm::mat4 MVP);
    void setCameraPos(const glm::vec3& Pos);
    void setColorTextureUnit(unsigned int textureUnit);
    void setBillboardSize(float billboardSize);
    void setView(const glm::mat4 view);
    
 private:
    GLuint locMVP;
    GLuint locCameraPos;
    GLuint locColorMap;
    GLuint locSize;
};


#endif	/* SPARKLER_RENDER_PROGRAM_H */