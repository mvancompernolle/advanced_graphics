#include "GBuffer.hpp"

#include <stdio.h>

using namespace Vancom;

GBuffer::GBuffer(){

	fbo = 0;
	depthTexture = 0;
}

GBuffer::~GBuffer(){

	if(fbo != 0)
		glDeleteFramebuffers(1, &fbo);
	if(textures[0] != 0)
		glDeleteTextures(GBUFFER_NUM_TEXTURES, textures);
	if(depthTexture != 0)
		glDeleteTextures(1, &depthTexture);
}

bool GBuffer::init(unsigned int windowWidth, unsigned int windowHeight){

	// create the FBO
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

	// create the gbuffer textures
	glGenTextures(GBUFFER_NUM_TEXTURES, textures);
	glGenTextures(1, &depthTexture);

    for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++) {
    	glBindTexture(GL_TEXTURE_2D, textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, NULL);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textures[i], 0);
    }	

    // depth buffer
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

   	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, 
						     GL_COLOR_ATTACHMENT1,
						     GL_COLOR_ATTACHMENT2,
						     GL_COLOR_ATTACHMENT3 };

	glDrawBuffers(4, drawBuffers);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE) {
        printf("FB error, status: 0x%x\n", status);
        return false;
    }

	// restore default FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	return true;
}

void GBuffer::bindForWriting(){

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
}

void GBuffer::bindForReading(){

	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
}

void GBuffer::setReadBuffer(GBUFFER_TEXTURE_TYPE textureType){

	glReadBuffer(GL_COLOR_ATTACHMENT0 + textureType);
}