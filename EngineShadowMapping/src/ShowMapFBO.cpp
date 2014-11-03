#include "ShadowMapFBO.hpp"

#include <stdio.h>

// SOURCE : http://ogldev.atspace.co.uk/www/tutorial23/tutorial23.html

using namespace Vancom;

ShadowMapFBO::ShadowMapFBO(){

	fbo = 0;
	shadowMapTexture = 0;
}

ShadowMapFBO::~ShadowMapFBO(){

	if(fbo != 0)
		glDeleteFramebuffers(1, &fbo);
	if(shadowMapTexture != 0)
		glDeleteTextures(1, &shadowMapTexture);
}

bool ShadowMapFBO::init(unsigned int windowWidth, unsigned int windowHeight){
	// create the fbo
	glGenFramebuffers(1, &fbo);

	// create the depth buffer
	glGenTextures(1, &shadowMapTexture);
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapTexture, 0);

    // disable write to the color buffer
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE) {
        printf("FB error, status: 0x%x\n", status);
        return false;
    }

	return true;
}

void ShadowMapFBO::bindForWriting(){

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
}

void ShadowMapFBO::bindForReading(GLenum textureUnit){

	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
}