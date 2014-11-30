#include "Grass.hpp"

#include <iostream>

using namespace Vancom;

Grass::Grass(Engine *engine, glm::mat4 model) : engine(engine){

    this->model = model;
}

Grass::~Grass(){

}

bool Grass::init(){

	// initialize render program
	if(!program.init())
		return false;

	// create vao
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// create vbo
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	return true;
}

void Grass::generateBlades(const std::vector<VertexTN>& verts){
    blades.clear();
    Vertex vert;
    int perc = 100;
    int index = 0;
    int minGreen = 100, maxGreen = 160, minRed = 28, maxRed = 140, minBlue = 0, maxBlue = 80; 
    std::random_device rd;
    std::default_random_engine rand(rd());
    std::uniform_int_distribution<int> distr(0, 100);
    int count = 0;
    for(const VertexTN& v: verts){
    	if(count == 100000)
    		break;
    	count++;

        if(distr(rand) < 100){
            vert.pos = v.pos;
            //std::cout << distr(rand) << std::endl;
            /*vert.color[0] = ((float)minRed + (maxRed - minRed)*(distr(rand)/(float)100))/255;
            vert.color[1] = ((float)minGreen +(maxGreen - minGreen)*(distr(rand)/(float)100))/255;
            vert.color[2] = ((float)minBlue +(maxBlue - minBlue)*(distr(rand)/(float)100))/255;
            vert.offset = (float) distr(rand) / 300;*/
            //std::cout << vert.offset << std::endl;
            blades.push_back(vert);
        }
        index++;
    }

    glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * blades.size(), &blades[0], GL_STATIC_DRAW);

	// setup attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0); // pos
}

void Grass::tick(float dt){

}

void Grass::render(){
}

void Grass::render(glm::mat4 projection, glm::mat4 view){

	program.enable();

	glBindVertexArray(vao);

	glEnableVertexAttribArray(0);

	glm::mat4 matrix;
	program.setMVP(projection * view * model);
	glDrawArrays(GL_POINTS, 0, blades.size());

	glDisableVertexAttribArray(0);

}