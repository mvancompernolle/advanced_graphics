#include "Water.h"
#include <iostream>

Water::Water(GLint program){
	this->program = program;

    loc_position = glGetAttribLocation(program,
                    const_cast<const char*>("v_position"));
    if(loc_position == -1)
    {
        std::cerr << "[F] POSITION NOT FOUND" << std::endl;
        //return false;
    }

    loc_mvpmat = glGetUniformLocation(program,
                    const_cast<const char*>("mvpMatrix"));
    if(loc_mvpmat == -1)
    {
        std::cerr << "[F] MVP NOT FOUND" << std::endl;
        //return false;
    }
}

void Water::createWaterMesh(float w, float h, float density){
	float xOffset = w/2, zOffset = h/2;
	Vertex vert;
	vert.pos[1] = 0;

	// loop to generate mesh
	for(float z = -zOffset; z < zOffset-density; z+=density){
		for(float x = -xOffset; x < xOffset-density; x+=density){
			// push 2 triangles
			vert.pos[0] = x;
			vert.pos[2] = z;
			vertices.push_back(vert);
			vert.pos[0] = x;
			vert.pos[2] = z+density;
			vertices.push_back(vert);
			vert.pos[0] = x+density;
			vert.pos[2] = z;
			vertices.push_back(vert);
			vert.pos[0] = x+density;
			vert.pos[2] = z;
			vertices.push_back(vert);
			vert.pos[0] = x;
			vert.pos[2] = z+density;
			vertices.push_back(vert);
			vert.pos[0] = x+density;
			vert.pos[2] = z+density;
			vertices.push_back(vert);
		}
	}

	// loop through triangles and calculate normals
	glm::vec3 normal;
	normal = 


	// create vbo for water
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

}

void Water::render(glm::mat4 pvMat){
    glm::mat4 mvp = pvMat * model;

    // enable the shader program
    glUseProgram(program);

    glEnableVertexAttribArray(loc_position);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //upload the matrix to the shader
    glUniformMatrix4fv(loc_mvpmat, 1, GL_FALSE, glm::value_ptr(mvp));

	glVertexAttribPointer( loc_position,//location of attribute
	                       3,//number of elements
	                       GL_FLOAT,//type
	                       GL_FALSE,//normalized?
	                       sizeof(Vertex),//stride
	                       0);//offset

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    
    glDisableVertexAttribArray(loc_position);
    glUseProgram(0);
}