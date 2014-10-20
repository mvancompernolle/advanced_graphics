#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <string>
#include <streambuf>

class ShaderLoader{
public:
	ShaderLoader();
	GLint loadShaderFromFile(GLenum shaderType, const char* fileName);
};

#endif // SHADER_LOADER_H
