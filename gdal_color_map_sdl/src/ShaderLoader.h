#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <string>
#include <streambuf>

class ShaderLoader{
public:
	ShaderLoader(GLenum shaderType);
	bool loadShaderFromFile(const char* fileName);
	GLint getShader();
private:
	GLenum typeOfShader;
	GLint shader;
};

#endif // SHADER_LOADER_H
