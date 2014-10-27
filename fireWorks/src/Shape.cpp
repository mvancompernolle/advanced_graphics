#include "Shape.h"
#include <iostream>

Shape::Shape(){
	textureImage = NULL;
	originX = 0;
	originY = 0;
}

bool Shape::init(const char* fileName, bool isBoundary){
	Vertex vert, vert2;
	double x, z;
	vert.position[1] = 0;
	vert2.position[1] = 0;
	ds = OGRSFDriverRegistrar::Open( fileName, FALSE );

	// Do a check
	if(ds == NULL)
	{
		std::cout << "Something went wrong." <<  std::endl;
		return false;
	}

	// Check how many layers there are.
    //std::cout << "Number of Layers " << ds->GetLayerCount() << std::endl;

    // Now lets grab the first layer
    OGRLayer* layer = ds->GetLayer(0);

	
    // Taking from http://www.compsci.wm.edu/SciClone/documentation/software/geo/gdal-1.9.0/html/ogr/ogr_apitut.html
    OGRFeature *poFeature;

    layer->ResetReading();
    while( (poFeature = layer->GetNextFeature()) != NULL )
    {
        OGRGeometry *poGeometry;

        poGeometry = poFeature->GetGeometryRef();

        // check to see if shape is in the boundary
        if(isBoundary){
            poGeometry = poGeometry->Boundary();
        }

        if( poGeometry != NULL 
            && wkbFlatten(poGeometry->getGeometryType()) == wkbPoint )
        {
            //OGRPoint *poPoint = (OGRPoint *) poGeometry;

            //printf( "%.3f,%3.f\n", poPoint->getX(), poPoint->getY() );
        }
        else if (poGeometry != NULL 
            && wkbFlatten(poGeometry->getGeometryType()) == wkbLineString)
        {
            //std::cout << "LINES!!!!" << std::endl;
            OGRLineString* ls= (OGRLineString*)poGeometry;
            lineSizes.push_back(ls->getNumPoints());
            for(int i = 0; i < ls->getNumPoints(); i++ )
            {
            	OGRPoint p;
            	ls->getPoint(i,&p);

            	x = originX -p.getX();
            	z = originY - p.getY();
            	vert.position[0] = -x * scale;
            	vert.position[2] = z * scale;
                shapeTriangles.push_back(vert);
            }
        }       
        OGRFeature::DestroyFeature( poFeature );
    }

    OGRDataSource::DestroyDataSource( ds );

    //std::cout << shapeTriangles.size() << std::endl;
	// create vbo for shape
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * shapeTriangles.size(), &shapeTriangles[0], GL_STATIC_DRAW);

	return true;
}

void Shape::setOrigin(double x, double y){
	originX = x;
	originY = y;
}

void Shape::setScale(double scale, float heightScale){
	//std::cout << "scale " << scale << std::endl;
	this->scale = scale;
    this->heightScale = heightScale;
}

double Shape::getScale(){
    return scale;
}

void Shape::render(glm::mat4 projection, glm::mat4 view){
    glm::mat4 mvp = projection * view * model;

    // enable the shader program
    glUseProgram(program);

    glEnableVertexAttribArray(loc_position);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //upload the matrix to the shader
    glUniformMatrix4fv(loc_mvpmat, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniform3fv(loc_color, 1, glm::value_ptr(color));
    glUniform1f(loc_scalar, heightScale);

	glVertexAttribPointer( loc_position,//location of attribute
	                       3,//number of elements
	                       GL_FLOAT,//type
	                       GL_FALSE,//normalized?
	                       sizeof(Vertex),//stride
	                       0);//offset

    glDrawArrays(GL_POINTS, 0, shapeTriangles.size());
    
    glDisableVertexAttribArray(loc_position);
    glUseProgram(0);
}

void Shape::setProgram(GLint prog){
	this->program = prog;

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

    loc_scalar = glGetUniformLocation(program,
                    const_cast<const char*>("verticalScalar"));
    if(loc_scalar == -1)
    {
        std::cerr << "[F] VERT SCALAR NOT FOUND" << std::endl;
        //return false;
    }

    loc_color = glGetUniformLocation(program,
                    const_cast<const char*>("color"));
    if(loc_color == -1)
    {
        std::cerr << "[F] COLOR NOT FOUND" << std::endl;
        //return false;
    }
}

void Shape::setColor(glm::vec3 color){
	this->color = color;
}

glm::vec3 Shape::getColor(){
    return color;
}

const std::vector<Vertex>& Shape::getShapeVertices(){
    return shapeTriangles;
}

const std::vector<int>& Shape::getLineSizes(){
    return lineSizes;
}

void Shape::setShapeVertices(std::vector<Vertex> verts){
    shapeTriangles = verts;
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * shapeTriangles.size(), &shapeTriangles[0], GL_STATIC_DRAW);
}