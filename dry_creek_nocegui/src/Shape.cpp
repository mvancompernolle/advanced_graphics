#include "Shape.h"
#include <iostream>

Shape::Shape(){
	textureImage = NULL;
	originX = 0;
	originY = 0;
}

bool Shape::init(const char* fileName){
	Vertex vert, vert2;
	double x, x2, z, z2;
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
    std::cout << "Number of Layers " << ds->GetLayerCount() << std::endl;

    // Now lets grab the first layer
    OGRLayer* layer = ds->GetLayer(0);

	
    // Taking from http://www.compsci.wm.edu/SciClone/documentation/software/geo/gdal-1.9.0/html/ogr/ogr_apitut.html
    OGRFeature *poFeature;

    layer->ResetReading();
    while( (poFeature = layer->GetNextFeature()) != NULL )
    {
        OGRFeatureDefn *poFDefn = layer->GetLayerDefn();
        int iField;

        for( iField = 0; iField < poFDefn->GetFieldCount(); iField++ )
        {
            OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( iField );

            /*if( poFieldDefn->GetType() == OFTInteger )
                printf( "%d,", poFeature->GetFieldAsInteger( iField ) );
            else if( poFieldDefn->GetType() == OFTReal )
                printf( "%.3f,", poFeature->GetFieldAsDouble(iField) );
            else if( poFieldDefn->GetType() == OFTString )
                printf( "%s,", poFeature->GetFieldAsString(iField) );
            else
                printf( "%s,", poFeature->GetFieldAsString(iField) );*/
        }

        OGRGeometry *poGeometry;

        poGeometry = poFeature->GetGeometryRef();
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
            for(int i = 0; i < ls->getNumPoints()-1; i++ )
            {
            	OGRPoint p, p2;
            	ls->getPoint(i,&p);
            	ls->getPoint(i+1,&p2);

            	x = originX -p.getX();
            	z = originY - p.getY();
            	x2 = originX -p2.getX();
            	z2 = originY - p2.getY();
            	vert.position[0] = -x * scale;
            	vert.position[2] = z * scale;
            	vert2.position[0] = -x2 * scale;
            	vert2.position[2] = z2 * scale;
            	//printf( "%.3f,%.3f\n", vert.position[0], vert.position[2] );
            	addLine(vert, vert2, scale*10);

            }
        }       
        OGRFeature::DestroyFeature( poFeature );
    }

    OGRDataSource::DestroyDataSource( ds );
    std::cout << shapeVertices.size() << std::endl;
    
    /*for(int i=0; i < shapeVertices.size()-1; i++){
    	//addLine(shapeVertices[i], shapeVertices[i+1], .1);
    }*/

    std::cout << shapeTriangles.size() << std::endl;
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

void Shape::setScale(double scale){
	std::cout << "scale " << scale << std::endl;
	this->scale = scale;
}

void Shape::addLine(Vertex p1, Vertex p2, float thickness){
	// http://stackoverflow.com/questions/101718/drawing-a-variable-width-line-in-opengl-no-gllinewidth
	Vertex vert1, vert2, vert3, vert4;
    float height = 1;
    float angle = atan2(p2.position[2] - p1.position[2], p2.position[0] - p1.position[0]);
    //std::cout << "angle " << angle << std::endl;
    float yOffset = thickness / 2 * sin(angle);
    float xOffset = thickness / 2 * cos(angle);
    //std::cout << "sin " << yOffset << " cos " << xOffset << std::endl;

    // create the 4 vertices
    vert1.position[0] = p1.position[0] - yOffset;
    vert1.position[1] = height;
    vert1.position[2] = p1.position[2] + xOffset;

    vert2.position[0] = p2.position[0] - yOffset;
    vert2.position[1] = height;
    vert2.position[2] = p2.position[2] + xOffset;

    vert3.position[0] = p2.position[0] + yOffset;
    vert3.position[1] = height;
    vert3.position[2] = p2.position[2] - xOffset;

    vert4.position[0] = p1.position[0] + yOffset;
    vert4.position[1] = height;
    vert4.position[2] = p1.position[2] - xOffset;

    /*std::cout << "p1 x: " << p1.position[0] << " y: " << p1.position[2] << std::endl;
    std::cout << "p2 x: " << p2.position[0] << " y: " << p2.position[2] << std::endl;
    std::cout << "v1 x: " << vert1.position[0] << " y: " << vert1.position[2] << std::endl;
    std::cout << "v2 x: " << vert2.position[0] << " y: " << vert2.position[2] << std::endl;
    std::cout << "v3 x: " << vert3.position[0] << " y: " << vert3.position[2] << std::endl;
    std::cout << "v4 x: " << vert4.position[0] << " y: " << vert4.position[2] << std::endl;*/

    shapeTriangles.push_back(vert1);
    shapeTriangles.push_back(vert2);
    shapeTriangles.push_back(vert3);
    shapeTriangles.push_back(vert1);
    shapeTriangles.push_back(vert3);
    shapeTriangles.push_back(vert4);
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
    glUniform1f(loc_scalar, 5);

	glVertexAttribPointer( loc_position,//location of attribute
	                       3,//number of elements
	                       GL_FLOAT,//type
	                       GL_FALSE,//normalized?
	                       sizeof(Vertex),//stride
	                       0);//offset

    glDrawArrays(GL_TRIANGLES, 0, shapeTriangles.size());
    
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