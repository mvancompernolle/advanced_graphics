#include "ColoredGUIRect.hpp"
#include "Texture.hpp"
#include "Engine.hpp"
#include "GameManager.hpp"

#include <iostream>

using namespace Vancom;

ColoredGUIRect::ColoredGUIRect(Engine* engine, bool mainBar): engine(engine), mainBar(mainBar){
}

ColoredGUIRect::~ColoredGUIRect(){

}

bool ColoredGUIRect::init(glm::vec3 color, glm::vec2 topLeft, glm::vec2 bottomRight){

    // create the geometry
    this->color = color;
    this->topLeft = topLeft;
    this->bottomRight = bottomRight;
    Vertex vert;

    vert.pos = glm::vec3(topLeft.x, topLeft.y, 0);
    box.push_back(vert);

    vert.pos = glm::vec3(topLeft.x, bottomRight.y, 0);
    box.push_back(vert);

    vert.pos = glm::vec3(bottomRight.x, topLeft.y, 0);
    box.push_back(vert);

    vert.pos = glm::vec3(bottomRight.x, bottomRight.y, 0);
    box.push_back(vert);

    // create vao
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // create vbo
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * box.size(), box.data(), GL_DYNAMIC_DRAW);

    // setup attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0); // pos

    return true;
}

void ColoredGUIRect::tick(float dt){

    if(mainBar){
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        box.clear();
        Vertex vert;

        float scaleVal = engine->gameManager->score/1000;
        float rightPos = scaleVal * .6;
        color = glm::vec3(1*(1-scaleVal), 1*scaleVal, 0);

        vert.pos = glm::vec3(topLeft.x, topLeft.y, 0);
        box.push_back(vert);

        vert.pos = glm::vec3(topLeft.x, bottomRight.y, 0);
        box.push_back(vert);

        vert.pos = glm::vec3(rightPos, topLeft.y, 0);
        box.push_back(vert);

        vert.pos = glm::vec3(rightPos, bottomRight.y, 0);
        box.push_back(vert);

        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * box.size(), box.data(), GL_STATIC_DRAW);
    }
}

void ColoredGUIRect::render(){

    // bind vao
    glBindVertexArray(vao);

    // draw
    glDrawArrays(GL_TRIANGLE_STRIP, 0, box.size());
}