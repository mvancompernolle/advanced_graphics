#include "Water.hpp"

#include "Engine.hpp"
#include "Texture.hpp"
#include "LightingManager.hpp"
#include "Lights.hpp"
#include "Graphics.hpp"
#include "Camera.hpp"

#include <random>
#include <iostream>

using namespace Vancom;

Water::Water(Engine *engine, float w, float h) : engine(engine), width(w), height(h){

    density = 50.0f;
}

Water::~Water(){

}

bool Water::init(){

    VertexT vert;
    vert.pos[1] = 0.0f;

    // initialize render program
    if(!program.init())
        return false;

    std::random_device rd;
    std::default_random_engine gen(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    // generate the water vertices
    for(float x = 0.0f; x < width; x += density) {
        for(float z = 0.0f; z < height; z += density) {
            vert.pos[0] = x;
            vert.pos[1] = 0 /*dist(gen)*/;
            vert.pos[2] = z;
            vert.tex[0] = x/width;
            vert.tex[1] = z/height;
            geometry.push_back(vert);
        }
    }

    // generate water indices
    unsigned int scaledWidth = width/density, scaledHeight = height/density;    
    for(int x = 0; x < scaledWidth - 1; x++) {
        for(int z = 0; z < scaledHeight - 1; z++) {
            indices.push_back(z * scaledHeight + x);
            indices.push_back((z*scaledHeight) + x + 1);
            indices.push_back((z+1) * scaledHeight + x);

            indices.push_back((z * scaledHeight) + x + 1);
            indices.push_back((z+1) * scaledHeight + x + 1);
            indices.push_back((z + 1) * scaledHeight + x);
        }
    }

    // translate terrain to the middle of the screen
    model = glm::translate(model, glm::vec3(-width/2.0f, -100.0f, -height/2.0f));

    // create vao
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // create vbo
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexT) * geometry.size(), geometry.data(), GL_STATIC_DRAW);

    // generate and use ibo for faster rendering
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // setup attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexT), 0); // pos
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexT), (const GLvoid*) 12); // tex

    return true;

}

bool Water::setTexture(GLenum TextureTarget, const char* fileName){

    // set texture
    waterTexture = new Texture(TextureTarget, fileName);

    if(!waterTexture->create()){
        std::cerr << "There was an error creating the water texture" << std::endl;
        return false;
    }

    return true;
}

void Water::tick(float dt){
    time += dt;
}

void Water::render(){

}

void Water::render(glm::mat4 projection, glm::mat4 view){

    program.enable();

    // bind vao
    glBindVertexArray(vao);

    // bind water texture
    waterTexture->bind(GL_TEXTURE0);

    program.setMVP(projection * view * model);
    program.setModel(model);
    program.setTime(time);
    program.setTextureUnit(0);
    program.setDirLight(engine->lightingManager->dirLight);
    program.setCameraPosition(engine->graphics->camera->getPos());

    // draw
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);

}