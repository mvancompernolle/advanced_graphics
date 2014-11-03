#ifndef TERRAINPROGRAM_HPP
#define	TERRAINPROGRAM_HPP

#include "ShaderProgram.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

namespace Vancom{

class TerrainProgram : public ShaderProgram{

public:

    // public functions
    TerrainProgram();
    bool init();    
    void setMVP(const glm::mat4 MVP) const;
    void setModelPos(const glm::mat4 model) const;
    void setSampler(const int sampler) const;

    // public variables
    GLuint locPos;
    GLuint locTex;
    GLuint locNormal;
    
private:

    // private variables
    GLuint locMVP;
    GLuint locModel;
    GLuint locSampler;
    
};

}

#endif	// TERRAINPROGRAM_HPP 