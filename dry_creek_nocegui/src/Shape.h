#ifndef SHAPE_H
#define	SHAPE_H

#include <GL/glew.h>
#include <FreeImagePlus.h>
#include "gdal_priv.h"
#include "Vertex.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier
#include <ogrsf_frmts.h>
#include <gdal.h>
#include <string>
#include <cpl_conv.h>

// REFERENCE: ogldev.atspace.co.uk/www/tutorial16/tutorial16.html

class Shape
{
public:
    Shape();
    bool init(const char* fileName);
    void setOrigin(double x, double y);
    void setScale(double scale);
    void render(glm::mat4 projection, glm::mat4 view);
    void addLine(Vertex point1, Vertex point2, float thickness);
    void setProgram(GLint program);
    void setColor(glm::vec3 color);

private:
    fipImage textureImage;
    OGRDataSource* ds;
    glm::mat4 model;
    GLint program, loc_position, loc_scalar, loc_mvpmat, loc_color;
    glm::vec3 color;
    double originX, originY, scale;
    std::vector<Vertex> shapeVertices, shapeTriangles;
    GLuint VBO;
};


#endif	/* SHAPE_H */