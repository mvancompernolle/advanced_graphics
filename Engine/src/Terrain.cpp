#include "Terrain.hpp"

#include <iostream>

#include "Engine.hpp"
#include "Graphics.hpp"
#include "Camera.hpp"
#include "PhysicsManager.hpp"

using namespace Vancom;

Terrain::Terrain(Engine *engine, const char* fileName) : engine(engine), fileName(fileName){
	specularIntensity = .5;
	specularPower = 32;
	downSample = 50;
	heightScale = 100;
	geotransform = new double[6];
	trimesh = new btTriangleMesh();
}

Terrain::~Terrain(){

	// delete geotransform 
	delete geotransform;

	if(trimesh != NULL)
		delete trimesh;
}

bool Terrain::init(){

	int bGotMin, bGotMax;
    double adfMinMax[2];
	GDALRasterBand* poBand;

	// get gdal data set from file
	gdalDataSet = (GDALDataset *) GDALOpen( fileName, GA_ReadOnly );
    if( gdalDataSet == NULL )
    {
        std::cout << "GDAL file failed to open!" << std::endl;
        return false;
    }

	// get height and width of mesh
	width = gdalDataSet->GetRasterXSize();
	height = gdalDataSet->GetRasterYSize();

	// git min max and range
    poBand = gdalDataSet->GetRasterBand( 1 );
    adfMinMax[0] = poBand->GetMinimum( &bGotMin );
    adfMinMax[1] = poBand->GetMaximum( &bGotMax );
    if( ! (bGotMin && bGotMax) )
        GDALComputeRasterMinMax((GDALRasterBandH)poBand, TRUE, adfMinMax);

	min = adfMinMax[0];
	max = adfMinMax[1];
	range = max - min;

	// get the geotransform values for the terrain
	gdalDataSet->GetGeoTransform(geotransform);

	// calculate heightscale so that it is accurate
	heightScale = range / geotransform[1];

    if(!generateMesh())
    	return false;

	return true;
}

bool Terrain::generateMesh(){

	float** data = new float*[height];
	float* terrain = new float[width*height];
	GDALRasterBand* poBand;
	VertexTN vert;

	// create a 2d array to store pixel values
	for(int i=0; i<height; i++){
		data[i] = new float[width];
	}
	poBand = gdalDataSet->GetRasterBand( 1 );
	// load all of the images height values into the data array
	for(int i=0; i<height; i++){
		poBand->RasterIO(GF_Read, 0, i, width, 1, data[i], width, 1, GDT_Float32, 0, 0);

		for(int j=0; j<width; j++)
			terrain[i*height + j] = data[i][j];
	}

	// generate terrain geometry
	for(int z = 0; z < height; z++){

		for(int x = 0; x < width; x++){

			// push vertex
			vert.pos.x = x;
			vert.pos.y = heightScale * ((data[z][x]-min)/range);
			vert.pos.z = z;
			vert.tex.x = vert.pos.x/100;
			vert.tex.y = vert.pos.z/100;

			// calculate the normal for the vertex
			calculateNormal(data, z, x, vert);

			geometry.push_back(vert);
		}
	}

	// generate terrain indices
	for(int z = 0; z < height-1; z++){

		for(int x = 0; x < width-1; x++){
			// first triangle
			indices.push_back((z * width) + x);
			indices.push_back(((z+1) * width) + x);
			indices.push_back((z * width) + x + 1);

			// second triangle
			indices.push_back((z * width) + x+1);
			indices.push_back(((z+1) * width) + x);
			indices.push_back(((z+1) * width) + x+1);
		}

	}

	model = glm::translate(model, glm::vec3(-width/2, 0, -height/2));

	// create vao
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// create vbo
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexTN) * geometry.size(), geometry.data(), GL_STATIC_DRAW);

	// generate and use ibo for faster rendering
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

	// setup attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTN), 0); // pos
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexTN), (const GLvoid*) 12); // tex
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTN), (const GLvoid*) 20); // normal

	/*// create collision shape
	for(unsigned int i=0; i<geometry.size(); i++){
		trimesh->addTriangle(engine->physics->convertTobtVector(geometry[i].pos), 
			engine->physics->convertTobtVector(geometry[i+1].pos), 
			engine->physics->convertTobtVector(geometry[i+2].pos));
		i+=2;
	}*/

	// create collision geometry
	btVector3 v1, v2, v3;
	for(int z = 0; z < height-downSample; z+=downSample){

		for(int x = 0; x < width-downSample; x+=downSample){

			// top triangle
			v1[0] = x;
			v1[1] = heightScale * ((data[z][x]-min)/range);
			v1[2] = z;
			v2[0] = x;
			v2[1] = heightScale * ((data[z+downSample][x]-min)/range);
			v2[2] = z;
			v3[0] = x;
			v3[1] = heightScale * ((data[z][x+downSample]-min)/range);
			v3[2] = z;
			trimesh->addTriangle(v1, v2, v3);

			// bottom triangle
			v1[0] = x;
			v1[1] = heightScale * ((data[z+downSample][x+downSample]-min)/range);
			v1[2] = z;
			v2[0] = x;
			v2[1] = heightScale * ((data[z+downSample][x]-min)/range);
			v2[2] = z;
			v3[0] = x;
			v3[1] = heightScale * ((data[z][x+downSample]-min)/range);
			v3[2] = z;
			trimesh->addTriangle(v1, v2, v3);
		}
	}

	//shape = new btHeightFieldTerrainShape(width, height, data, max, min, 1, false, false);
	shape = new btConvexTriangleMeshShape(trimesh);
	//shape = new btBvhTriangleMeshShape(trimesh, true, true);

	// add terrain physics to the dynamics world
	rigidBody = engine->physics->addRigidBody(shape, btQuaternion(0,0,0,1), btVector3(-width/2,0,-height/2), 0.0f, 1.0f, btVector3(0,0,0));
	//rigidBody->setActivationState(DISABLE_DEACTIVATION);
	//rigidBody->setGravity(btVector3(0.0, 0.0, 0.0));

	// delete pixel data
	for(int i=0; i<height; i++)
		delete data[i];
	delete data;

	delete terrain;

	return true;
}

void Terrain::calculateNormal(float **data, int z, int x, VertexTN& vert){
	glm::vec3 normal, center, up, down, left, right;

	// get temp center vertex
	center = vert.pos;

	// calculate the normal for a non edge vertex
	if(x > 0 && z > 0 && x < width-1 && z < height-1){

		// up vertex
		up.z = z+1;
		up.x = x;
		up.y = heightScale * ((data[z+1][x]-min)/range);

		// down vertex
		down.z = z-1;
		down.x = x;
		down.y = heightScale * ((data[z-1][x]-min)/range);

		// left vertex
		left.z = z;
		left.x = x-1;
		left.y = heightScale * ((data[z][x-1]-min)/range);

		// right vertex
		right.z = z;
		right.x = x+1;
		right.y = heightScale * ((data[z][x+1]-min)/range);

		// sum normals and average them
		normal += glm::cross(up - center, right - center);
		normal += glm::cross(right - center, down - center);
		normal += glm::cross(down - center, left - center);
		normal += glm::cross(left - center, up - center);
		normal = glm::normalize(normal);

		// set calculated normals on the given vertex
		vert.normal = normal;
	}
}

bool Terrain::setTexture(GLenum TextureTarget, const char* fileName){

	// set texture
	groundTexture = new Texture(TextureTarget, fileName);

	if(!groundTexture->create()){
		std::cerr << "There was an error creating the terrain texture" << std::endl;
		return false;
	}

	return true;
}

void Terrain::tick(float dt){

    btTransform trans;
	btScalar m[16];
	rigidBody->getMotionState()->getWorldTransform(trans);
	trans.getOpenGLMatrix(m);
	model = glm::make_mat4(m);
}

void Terrain::render(){

	// bind vao
	glBindVertexArray(vao);

	// enable attributes
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// bind ground texture
	groundTexture->bind(GL_TEXTURE0);

	// draw
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);

	// disable attributes
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Terrain::getDimensions(int& width, int& height) const{
	
	width = this->width;
	height = this->height;
}

std::vector<VertexTN> Terrain::getGeometry() const{
	
	return geometry;
}

float** Terrain::getHeightMap() const{
	float** data = new float*[height];
	GDALRasterBand* poBand;

	// create a 2d array to store pixel values
	for(int i=0; i<height; i++){
		data[i] = new float[width];
	}
	poBand = gdalDataSet->GetRasterBand( 1 );
	// load all of the images height values into the data array
	for(int i=0; i<height; i++){
		poBand->RasterIO(GF_Read, 0, i, width, 1, data[i], width, 1, GDT_Float32, 0, 0);
	}

	// normalize all the values
	for(int i=0; i<height; i++)
		for(int j=0; j<width; j++)
			data[i][j] = heightScale * (data[i][j] - min) / range;

	return data;
}
