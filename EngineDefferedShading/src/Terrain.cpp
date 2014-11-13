#include "Terrain.hpp"

#include <iostream>

#include "Engine.hpp"
#include "Graphics.hpp"
#include "Camera.hpp"

using namespace Vancom;

Terrain::Terrain(Engine *engine, const char* fileName) : engine(engine), fileName(fileName){
	heightScale = 100;
}

Terrain::~Terrain(){

}

bool Terrain::init(){

	int bGotMin, bGotMax;
    double adfMinMax[2];
	GDALRasterBand* poBand;

	// initalize G buffer
	int w, h;
	engine->graphics->getWindowSize(w, h);
	if(!buffer.init(w, h)){
		return false;
	}
	
	// initalize geometry program
	if(!program.init())
		return false;

	program.enable();
	program.setColorTextureUnit(0);

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

	// translate terrain to the middle of the screen
	model = glm::translate(model, glm::vec3(-width/2.0f, 0.0f, -height/2.0f));

	// git min max and range
    poBand = gdalDataSet->GetRasterBand( 1 );
    adfMinMax[0] = poBand->GetMinimum( &bGotMin );
    adfMinMax[1] = poBand->GetMaximum( &bGotMax );
    if( ! (bGotMin && bGotMax) )
        GDALComputeRasterMinMax((GDALRasterBandH)poBand, TRUE, adfMinMax);

	min = adfMinMax[0];
	max = adfMinMax[1];
	range = max - min;

    if(!generateMesh())
    	return false;

    // initialize directional light program
	if(!dlProgram.init())
		return false;

	dlProgram.enable();   
	dlProgram.setPositionTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	dlProgram.setColorTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
	dlProgram.setNormalTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
	dlProgram.setLightDirection(engine->graphics->getLightDirection());
    dlProgram.setScreenSize(w, h);

	// initialize mesh  
	dirLightRenderQuad = new Mesh(program);
	dirLightRenderQuad->loadMesh("../assets/models/quad.obj");

	return true;
}

bool Terrain::generateMesh(){

	float** data = new float*[height];
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
	}

	// generate terrain geometry
	for(int z = 0; z < height; z++){

		for(int x = 0; x < width; x++){

			// push vertex
			vert.pos.x = x;
			vert.pos.y = heightScale * ((data[z][x]-min)/range);
			vert.pos.z = z;
			vert.tex.x = vert.pos.x/200;
			vert.tex.y = vert.pos.z/200;

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
			indices.push_back((z * width) + x + 1);
			indices.push_back(((z+1) * width) + x);

			// second triangle
			indices.push_back((z * width) + x+1);
			indices.push_back(((z+1) * width) + x+1);
			indices.push_back(((z+1) * width) + x);
		}

	}

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

	// enable attributes
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// setup attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTN), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexTN), (const GLvoid*) 12);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTN), (const GLvoid*) 20);

	// delete pixel data
	for(int i=0; i<height; i++)
		delete data[i];
	delete data;

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


}

void Terrain::geometryPass(glm::mat4 projection, glm::mat4 view){

	program.enable();
	glBindVertexArray(vao);

	buffer.bindForWriting();

	// only the geometry pass updates the depth buffer
	glDepthMask(GL_TRUE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	// set uniforms
	glm::mat4 mvp = projection * view * model;
	program.setMVP(mvp);
	program.setModelPos(model);

	// bind ground texture
	groundTexture->bind(GL_TEXTURE0);

	// draw
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);

    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);
}

void Terrain::beginLightPasses(){

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	buffer.bindForReading();
	glClear(GL_COLOR_BUFFER_BIT);
}

void Terrain::directionalLightPass(glm::mat4 projection, glm::mat4 view){
	
	// enable directional light program
	dlProgram.enable();

	// set directional light
	dlProgram.setLightDirection(engine->graphics->getLightDirection());

	// set mvp to identity matrix so the program is in screen space
	glm::mat4 mvp;
	dlProgram.setMVP(mvp);

	// render the whole screen
	dirLightRenderQuad->renderMesh();
}

void Terrain::render(glm::mat4 projection, glm::mat4 view){

	geometryPass(projection, view);
	beginLightPasses();
	directionalLightPass(projection, view);

}

void Terrain::getDimensions(int& width, int& height) const{
	
	width = this->width;
	height = this->height;
}
