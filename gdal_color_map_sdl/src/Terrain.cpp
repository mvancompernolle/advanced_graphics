#include "Terrain.h"
#include <iostream>

Terrain::Terrain(){

}

bool Terrain::loadHeightMap(const char* fileName, float scale){
	// get terrain texture
	numTriangles = 0;
	this->scale = scale;
	heightMapImage = new Texture(GL_TEXTURE_2D, fileName);

	if(heightMapImage->load()){
		generateMesh();
		return true;
	}
	else{
		return false;
	}
}

int Terrain::getWidth(){
	return width;
}

int Terrain::getHeight(){
	return height;
}

bool Terrain::generateMesh(){
	// get height and width of text from dgal
	GDALDataset* gdalImage = heightMapImage->getGDALImage();

	width = gdalImage->GetRasterXSize();
	height = gdalImage->GetRasterYSize();
	Vertex vert;
	int xOffset, yOffset;
	vert.position[1] = 0;

	GDALRasterBand  *poBand;
	poBand = gdalImage->GetRasterBand(1);
	int bandXSize = poBand->GetXSize();
	float* poScanLine = (float *) CPLMalloc(sizeof(float)*bandXSize);
	float* poScanLine2 = (float *) CPLMalloc(sizeof(float)*bandXSize);

	// generate a planar triangle mesh that corresponds with the texture
	xOffset = width/2;
	yOffset = height/2;


	for(int y=-yOffset; y<height-yOffset-1; y++){
		// get to rows of pixel values
		poBand->RasterIO(GF_Read, 0, y+yOffset, bandXSize, 1, poScanLine, bandXSize, 1, GDT_Float32, 0, 0);
		poBand->RasterIO(GF_Read, 0, y+yOffset+1, bandXSize, 1, poScanLine2, bandXSize, 1, GDT_Float32, 0, 0);

		for(int x=-xOffset; x<width-xOffset-1; x++){
				// push top row of triangles
				vert.position[0] = x*scale;
				vert.position[1] = poScanLine[x+xOffset]/255;
				/*if(x + xOffset == 30)
				std::cout << vert.position[1] << std::endl;*/
				vert.position[2] = y*scale;
				//vert.texture[0] = (x+xOffset)/width;
				//vert.texture[1] = (y+yOffset)/height;
				planeVertices.push_back(vert);
				vert.position[0] = (x+1)*scale;
				vert.position[1] = poScanLine[x+xOffset+1]/255;
				vert.position[2] = y*scale;
				//vert.texture[0] = (x+xOffset+1)/width;
				//vert.texture[1] = (y+yOffset)/height;
				planeVertices.push_back(vert);
				vert.position[0] = x*scale;
				vert.position[1] = poScanLine2[x+xOffset]/255;
				vert.position[2] = (y+1)*scale;
				//vert.texture[0] = (x+xOffset)/width;
				//vert.texture[1] = (y+yOffset+1)/height;
				planeVertices.push_back(vert);

				// push bottom row of triangles
				vert.position[0] = x*scale;
				vert.position[1] = poScanLine2[x+xOffset]/255;
				vert.position[2] = (y+1)*scale;
				//vert.texture[0] = (x+xOffset)/width;
				//vert.texture[1] = (y+yOffset+1)/height;
				planeVertices.push_back(vert);
				vert.position[0] = (x+1)*scale;
				vert.position[1] = poScanLine2[x+1+xOffset]/255;
				vert.position[2] = (y+1)*scale;
				//vert.texture[0] = (x+xOffset+1)/width;
				//vert.texture[1] = (y+yOffset+1)/height;
				planeVertices.push_back(vert);
				vert.position[0] = (x+1)*scale;
				vert.position[1] = poScanLine[x+xOffset+1]/255;
				vert.position[2] = y*scale;
				//vert.texture[0] = (x+xOffset+1)/width;
				//vert.texture[1] = (y+yOffset)/height;
				planeVertices.push_back(vert);		
				numTriangles += 2;	
		}
	}

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * planeVertices.size(), &planeVertices[0], GL_STATIC_DRAW);

	return true;
}

void Terrain::render(GLint loc_position, GLint loc_normal, GLint loc_texture){
    glEnableVertexAttribArray(loc_position);
    //glEnableVertexAttribArray(loc_texture);
    glEnableVertexAttribArray(loc_normal);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer( loc_position,//location of attribute
	                       3,//number of elements
	                       GL_FLOAT,//type
	                       GL_FALSE,//normalized?
	                       sizeof(Vertex),//stride
	                       0);//offset

	/*glVertexAttribPointer( loc_texture,//location of attribute
	                       2,//number of elements
	                       GL_FLOAT,//type
	                       GL_FALSE,//normalized?
	                       sizeof(Vertex),//stride
	                       (void*)offsetof(Vertex, texture));//offset*/

	glVertexAttribPointer( loc_normal,
	                       3,
	                       GL_FLOAT,
	                       GL_FALSE,
	                       sizeof(Vertex),
	                       (void*)offsetof(Vertex,normal));

    glDrawArrays(GL_TRIANGLES, 0, numTriangles * 3);
    

    glDisableVertexAttribArray(loc_position);
    //glDisableVertexAttribArray(loc_texture);
    glDisableVertexAttribArray(loc_normal);

}
