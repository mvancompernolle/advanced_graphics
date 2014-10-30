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
    void setSampler(const int sampler) const;
    void setLightDirection(const glm::vec3 direction) const;

    // public variables
    GLuint locMVP;
    GLuint locLightDir;
    GLuint locSampler;
    GLuint locPos;
    GLuint locTex;
    GLuint locNormal;
    
private:

};

}

#endif	// TERRAINPROGRAM_HPP 