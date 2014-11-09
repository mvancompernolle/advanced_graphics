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
    void setLightDirection(const glm::vec3 direction) const;
    void setSpotLightDirection(const glm::vec3 direction) const;
    void setSpotLightPosition(const glm::vec3 pos) const;
    void setSpecularPower(const float power) const;
    void setSpecularIntensity(const float intensity) const;
    void setCameraPosition(const glm::vec3 pos) const;
    void setSpecularFlag(const bool isOn) const;

    // public variables
    GLuint locMVP;
    GLuint locModel;
    GLuint locLightDir;
    GLuint locSpotLightPos;
    GLuint locSpotLightDir;
    GLuint locCameraPos;
    GLuint locSampler;
    GLuint locSpecularPower;
    GLuint locSpecularIntensity;
    GLuint locSpecularOn;
    GLuint locPos;
    GLuint locTex;
    GLuint locNormal;
    
private:

};

}

#endif	// TERRAINPROGRAM_HPP 