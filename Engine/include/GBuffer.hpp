#ifndef GBUFFER_HPP
#define GBUFFER_HPP

// SOURCE : http://ogldev.atspace.co.uk/www/tutorial37/tutorial37.html

#include <GL/glew.h>

namespace Vancom{

class GBuffer{

public:

	enum GBUFFER_TEXTURE_TYPE {
        GBUFFER_TEXTURE_TYPE_POSITION,
        GBUFFER_TEXTURE_TYPE_DIFFUSE,
        GBUFFER_TEXTURE_TYPE_NORMAL,
        GBUFFER_TEXTURE_TYPE_SPECULAR,
        GBUFFER_NUM_TEXTURES
    };

    // public funcitons
    GBuffer();
    ~GBuffer();
    bool init(unsigned int windowWidth, unsigned int windowHeight);
    void startFrame();
    void bindForGeometryPass();
    void bindForStencilPass();
    void bindForLightPass();
    void bindForFinalPass();  

private: 

	// private variables
	GLuint fbo;
	GLuint textures[GBUFFER_NUM_TEXTURES];
	GLuint depthTexture;
    GLuint finalTexture;

};

} // end namespcae Vancom

#endif // END GBUFFER_HPP