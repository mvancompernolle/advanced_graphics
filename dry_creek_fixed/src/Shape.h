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
#include <cmath>

// REFERENCE: ogldev.atspace.co.uk/www/tutorial16/tutorial16.html

class Shape
{
public:
    Shape();
    bool init(const char* fileName, bool isBoundary);
    void setOrigin(double x, double y);
    void setScale(double scale, float heightScale);
    double getScale();
    void render(glm::mat4 projection, glm::mat4 view);
    void addLine(Vertex point1, Vertex point2, float thickness);
    void setProgram(GLint program);
    void setColor(glm::vec3 color);
    glm::vec3 getColor();
    void setShapeVertices(std::vector<Vertex> verts);
    const std::vector<Vertex>& getShapeVertices();
    const std::vector<int>& getLineSizes();

private:
    fipImage textureImage;
    OGRDataSource* ds;
    glm::mat4 model;
    GLint program, loc_position, loc_scalar, loc_mvpmat, loc_color;
    glm::vec3 color;
    double originX, originY, scale;
    float heightScale;
    std::vector<Vertex> shapeTriangles;
    std::vector<int> lineSizes;
    GLuint VBO;
};


#endif	/* SHAPE_H */