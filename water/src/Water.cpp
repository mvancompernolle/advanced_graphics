#include "Water.h"
#include <iostream>

Water::Water(GLint program){
	this->program = program;

    loc_position = glGetAttribLocation(program,
                    const_cast<const char*>("v_position"));
    if(loc_position == -1)
    {
        std::cerr << "[F] POSITION NOT FOUND" << std::endl;
    }

    loc_normal = glGetAttribLocation(program,
                    const_cast<const char*>("v_normal"));
    if(loc_normal == -1)
    {
        std::cerr << "[F] NORMAL NOT FOUND" << std::endl;
    }


    loc_mvpmat = glGetUniformLocation(program,
                    const_cast<const char*>("mvpMatrix"));
    if(loc_mvpmat == -1)
    {
        std::cerr << "[F] MVP NOT FOUND" << std::endl;
    }

    loc_model = glGetUniformLocation(program,
                    const_cast<const char*>("model"));
    if(loc_model == -1)
    {
        std::cerr << "[F] MODEL NOT FOUND" << std::endl;
    }

    loc_view = glGetUniformLocation(program,
                    const_cast<const char*>("cameraPos"));
    if(loc_model == -1)
    {
        std::cerr << "[F] VIEW NOT FOUND" << std::endl;
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
	calculateNormals();

	// create vbo for water
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

}

void Water::calculateNormals(){
	GLfloat product[3];
	float length;
	// loop through each triangle and calculate normal
	for(int i=0; i<vertices.size(); i+=3){
		/*product[0] = (vertices[i].pos[1] * vertices[i+1].pos[2]) - (vertices[i].pos[2] * vertices[i+1].pos[1]);
		product[1] = -((vertices[i].pos[2] * vertices[i+1].pos[0]) - (vertices[i].pos[0] * vertices[i+1].pos[2]));
		product[2] = (vertices[i].pos[0] * vertices[i+1].pos[1]) - (vertices[i].pos[1] * vertices[i+1].pos[0]);
		length = sqrt(pow(product[0],2) + pow(product[1],2) + pow(product[2],2));
		product[0] /= length;
		product[1] /= length;
		product[2] /= length;
		//std::cout << product[0] << ' ' << product[1] << ' ' << product[2] << std::endl;*/

		for(int j=0; j<3; j++){
			vertices[i].norm[j] = 0;
			vertices[i+1].norm[j] = 1;
			vertices[i+2].norm[j] = 0;
		}



	/*if(product[0] != 0 || product[1] != 1 ||product[2] != 0)
		std::cout << product[0] << ' ' << product[1] << ' ' << product[2] << std::endl;*/
	}

}

void Water::render(glm::mat4 projection, glm::mat4 view){
    glm::mat4 mvp = projection * view * model;

    // enable the shader program
    glUseProgram(program);

    glEnableVertexAttribArray(loc_position);
    glEnableVertexAttribArray(loc_normal);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //upload the matrix to the shader
    glUniformMatrix4fv(loc_mvpmat, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniformMatrix4fv(loc_model, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(loc_view, 1, GL_FALSE, glm::value_ptr(view));

	glVertexAttribPointer( loc_position,//location of attribute
	                       3,//number of elements
	                       GL_FLOAT,//type
	                       GL_FALSE,//normalized?
	                       sizeof(Vertex),//stride
	                       0);//offset

	glVertexAttribPointer( loc_normal,//location of attribute
	                       3,//number of elements
	                       GL_FLOAT,//type
	                       GL_FALSE,//normalized?
	                       sizeof(Vertex),//stride
	                       (void*)offsetof(Vertex,norm));//offset

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    
    glDisableVertexAttribArray(loc_position);
    glDisableVertexAttribArray(loc_normal);
    glUseProgram(0);
}