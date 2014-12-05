#include "Grass.hpp"

#include <math.h>
#include <iostream>
#include "Engine.hpp"
#include "Graphics.hpp"
#include "Camera.hpp"

#define PI 3.14159265

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

void Grass::generateBlades(float** terrain, int width, int height){
    blades.clear();
    VertexGrass vert;
    int perc = 100;
    int index = 0;
    int minGreen = 100, maxGreen = 160, minRed = 28, maxRed = 140, minBlue = 0, maxBlue = 80; 
    int ax, az, bx, bz, cx, cz;

    std::random_device rd;
    std::default_random_engine rand(rd());
    std::uniform_int_distribution<int> distr(0, 100);
    std::uniform_real_distribution<float> xDistr(0, width-1);
    std::uniform_real_distribution<float> zDistr(0, height-1);
    std::uniform_real_distribution<float> distr2(0, 1.57);
    
    // generate blades of grass
    for(int i=0; i<1000000; i++){
        /*// generate position
        vert.pos.x = (int) xDistr(rand);
        vert.pos.z = (int) zDistr(rand);
        vert.pos.y = terrain[(int) vert.pos.z][(int) vert.pos.x];
        std::cout << vert.pos.x << " " << vert.pos.y << " " << vert.pos.z << std::endl;*/

        vert.pos.x = xDistr(rand);
        vert.pos.z = zDistr(rand);

        float zRemainder = vert.pos.z - (long) vert.pos.z;
        float xRemainder = vert.pos.x - (long) vert.pos.x;

        // find triangle point is in
        //std::cout << atan(xRemainder / zRemainder) * 180 / PI << std::endl;
        if(atan(xRemainder / -zRemainder) * 180 / PI < 45.0f){
            ax = vert.pos.x;
            az = vert.pos.z;
            bx = vert.pos.x;
            bz = vert.pos.z + 1;
            cx = vert.pos.x + 1;
            cz = vert.pos.z + 1;
            vert.pos.y = terrain[az][ax]+ zRemainder * (terrain[bz][bx] - terrain[az][ax])
            + xRemainder * (terrain[cz][cx] - terrain[az][ax]);
        }
        else{
            ax = vert.pos.x / 1;
            az = vert.pos.z / 1;
            bx = vert.pos.x / 1 + 1;
            bz = vert.pos.z / 1;
            cx = vert.pos.x / 1 + 1;
            cz = vert.pos.z / 1 + 1;
            vert.pos.y = terrain[az][ax]+ zRemainder * (terrain[bz][bx] - terrain[az][ax])
            + xRemainder * (terrain[cz][cx] - terrain[az][ax]);
        }
        //std::cout << vert.pos.x << " " << vert.pos.y << " " << vert.pos.z << std::endl;
        // find point on the triangle

        // generate random color
        vert.color[0] = ((float)minRed + (maxRed - minRed)*(distr(rand)/(float)100))/255;
        vert.color[1] = ((float)minGreen +(maxGreen - minGreen)*(distr(rand)/(float)100))/255;
        vert.color[2] = ((float)minBlue +(maxBlue - minBlue)*(distr(rand)/(float)100))/255;
        vert.angle = distr2(rand);

        blades.push_back(vert);
    }

    /*for(const VertexTN& v: verts){
        if(distr(rand) < 100){
            vert.pos = v.pos;

            // generate random color
            vert.color[0] = ((float)minRed + (maxRed - minRed)*(distr(rand)/(float)100))/255;
            vert.color[1] = ((float)minGreen +(maxGreen - minGreen)*(distr(rand)/(float)100))/255;
            vert.color[2] = ((float)minBlue +(maxBlue - minBlue)*(distr(rand)/(float)100))/255;
            vert.angle = distr2(rand);

            blades.push_back(vert);
        }
        index++;
    }*/

    glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexGrass) * blades.size(), &blades[0], GL_STATIC_DRAW);

	// setup attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexGrass), 0); // pos

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexGrass), (void*) 12); // color

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(VertexGrass), (void*) 24); // color
}

void Grass::tick(float dt){

}

void Grass::render(){
}

void Grass::render(glm::mat4 projection, glm::mat4 view){

	program.enable();

	glBindVertexArray(vao);

    //program.setCameraPos(engine->graphics->camera->getPos());
    program.setWindDir(engine->graphics->windDir);
	program.setMVP(projection * view * model);
    program.setCameraPos(engine->graphics->camera->getPos());
	glDrawArrays(GL_POINTS, 0, blades.size());

}