#include <iostream>
#include "Grass.h"

// REFERENCE: ogldev.atspace.co.uk/www/tutorial16/tutorial16.html

Grass::Grass(GLuint prog, float percent, glm::vec3 color)
{
    this->program = prog;
    this->percent = percent * 2/3;
    this->color = color;
    loc_position = glGetAttribLocation(program, "v_position");
    loc_color = glGetUniformLocation(program, "color");
    loc_hscalar = glGetUniformLocation(program, "verticalScalar");
    loc_mvp = glGetUniformLocation(program, "mvpMatrix");

    // create vbo for grass
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

}

Grass::~Grass(){

}

void Grass::sampleVertices(const std::vector<Vertex>& verts){
    vertices.clear();
    int perc = percent * 100;
    int index = 0;
    std::default_random_engine rand;
    std::uniform_int_distribution<int> distr(0, 100);
    for(Vertex v: verts){
        if(distr(rand) < perc && index % 3 != 0 && index %4 != 0){
            vertices.push_back(v);
        }
        index++;
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
}

void Grass::render(glm::mat4 pvMat){
    glm::mat4 mvp = pvMat * model;

    // enable the shader program
    glUseProgram(program);

    glEnableVertexAttribArray(loc_position);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //upload the matrix to the shader
    glUniformMatrix4fv(loc_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniform3fv(loc_color, 1, glm::value_ptr(color));

    glVertexAttribPointer( loc_position,//location of attribute
                           3,//number of elements
                           GL_FLOAT,//type
                           GL_FALSE,//normalized?
                           sizeof(Vertex),//stride
                           0);//offset

    glDrawArrays(GL_POINTS, 0, vertices.size());
    
    glDisableVertexAttribArray(loc_position);
    glUseProgram(0);
}