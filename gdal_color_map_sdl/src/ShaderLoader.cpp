#include "ShaderLoader.h"

ShaderLoader::ShaderLoader(GLenum shaderType){
	typeOfShader = shaderType;
}

bool ShaderLoader::loadShaderFromFile(const char* fileName){
	// initialize variables
    std::ifstream fin(fileName);
	GLint shader_status;
	std::string buf;
	const char * shaderString;

	// read shader from file
	if(fin){
		// seek to the end of the file to get its size
		fin.seekg(0, std::ios::end);
		long size = fin.tellg();
		fin.seekg(0, std::ios::beg);

		// dynamically allocate memory space for the shader string based on file size
		buf.reserve(size);

		// fill string with contents of file
		std::istreambuf_iterator<char> iterator = std::istreambuf_iterator<char>(fin);
		std::istreambuf_iterator<char> iteratorEnd = std::istreambuf_iterator<char>();
		buf.assign(iterator, iteratorEnd);
  		shaderString = buf.c_str();
		fin.close();

		// create and compile the shader
		shader = glCreateShader(typeOfShader);

		glShaderSource(shader, 1, &shaderString, NULL);
		glCompileShader(shader);
		//check the compile status
		glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_status);
		if(!shader_status)
		{
			if(typeOfShader == GL_VERTEX_SHADER)
				std::cerr << "[F] FAILED TO COMPILE VERTEX SHADER!" << std::endl;
			else
				std::cerr << "[F] FAILED TO COMPILE FRAGMENT SHADER!" << std::endl;
		    
		    return false;
		}

		return true;
	}
	else{
        std::cerr << "[F] FAILED TO OPEN SHADER FILE: " << fileName << std::endl;
        return false;
	}
}

GLint ShaderLoader::getShader(){
    return shader;
}

