#ifndef GBUFFER_HPP
#define GBUFFER_HPP

// SOURCE : http://ogldev.atspace.co.uk/www/tutorial35/tutorial35.html

#include <GL/glew.h>

#define GBUFFER_POSITION_TEXTURE_UNIT 0
#define GBUFFER_DIFFUSE_TEXTURE_UNIT  1
#define GBUFFER_NORMAL_TEXTURE_UNIT   2
#define GBUFFER_TEXCOORD_TEXTURE_UNIT 3

namespace Vancom{

class GBuffer{

public:

	enum GBUFFER_TEXTURE_TYPE {
        GBUFFER_TEXTURE_TYPE_POSITION,
        GBUFFER_TEXTURE_TYPE_DIFFUSE,
        GBUFFER_TEXTURE_TYPE_NORMAL,
        GBUFFER_TEXTURE_TYPE_TEXCOORD,
        GBUFFER_NUM_TEXTURES
    };

    // public funcitons
    GBuffer();
    ~GBuffer();
    bool init(unsigned int windowWidth, unsigned int windowHeight);
    void bindForWriting();
    void bindForReading();  
    void setReadBuffer(GBUFFER_TEXTURE_TYPE textureType);

private: 

	// private variables
	GLuint fbo;
	GLuint textures[GBUFFER_NUM_TEXTURES];
	GLuint depthTexture;

};

} // end namespcae Vancom

#endif // END GBUFFER_HPP