#include "Terrain.h"
#include <iostream>

Terrain::Terrain(){

}

bool Terrain::loadHeightMap(const char* fileName, float scale){
	// get terrain texture
	numTriangles = 0;
	this->scale = scale;
	heightMapImage = new Texture(GL_TEXTURE_2D, fileName);

	if(heightMapImage->Create()){
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
	fipImage image = heightMapImage->getImage();
	width = image.getWidth();
	height = image.getHeight();
	Vertex vert;
	float xOffset, yOffset;
	vert.position[1] = 0;

	// generate a planar triangle mesh that corresponds with the texture
	xOffset = image.getWidth()/2;
	yOffset = image.getHeight()/2;
	for(int y=-yOffset; y<image.getHeight()-yOffset; y++){
		for(int x=-xOffset; x<image.getWidth()-xOffset; x++){
				// push top row of triangles
				vert.position[0] = x*scale;
				vert.position[2] = y*scale;
				planeVertices.push_back(vert);
				vert.position[0] = x*scale+1;
				vert.position[2] = y*scale;
				planeVertices.push_back(vert);
				vert.position[0] = x*scale;
				vert.position[2] = y*scale+1;
				planeVertices.push_back(vert);

				// push bottom row of triangles
				vert.position[0] = x*scale;
				vert.position[2] = y*scale+1;
				planeVertices.push_back(vert);
				vert.position[0] = x*scale+1;
				vert.position[2] = y*scale+1;
				planeVertices.push_back(vert);
				vert.position[0] = x*scale+1;
				vert.position[2] = y*scale;
				planeVertices.push_back(vert);		
				numTriangles += 2;	
		}
	}

	std::cout << planeVertices.size();
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * planeVertices.size(), &planeVertices[0], GL_STATIC_DRAW);

	/*// get the heighmap image for the terrain
	fipImage image = heightMapImage->getImage();
	RGBQUAD pixel;

	// loop through the image and get each pixel value
	std::cout << image.getWidth() << " " << image.getHeight() << std::endl;
	for(int x=0; x<image.getWidth(); x++){
		for(int y=image.getHeight()-1; y>=0; y--){
			image.getPixelColor(x, 30, &pixel);
			//std::cout << "Shade: " << (int)pixel.rgbRed << " " << (int)pixel.rgbBlue << " " <<(int)pixel.rgbGreen << " " << (int)pixel.rgbReserved << std::endl;
		}
	}*/

	return true;
}

void Terrain::render(GLint loc_position, GLint loc_normal){
    glEnableVertexAttribArray(loc_position);
    glEnableVertexAttribArray(loc_normal);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer( loc_position,//location of attribute
	                       3,//number of elements
	                       GL_FLOAT,//type
	                       GL_FALSE,//normalized?
	                       sizeof(Vertex),//stride
	                       0);//offset

	glVertexAttribPointer( loc_normal,
	                       3,
	                       GL_FLOAT,
	                       GL_FALSE,
	                       sizeof(Vertex),
	                       (void*)offsetof(Vertex,normal));

    glDrawArrays(GL_TRIANGLES, 0, numTriangles);
    

    glDisableVertexAttribArray(loc_position);
    glDisableVertexAttribArray(loc_normal);

}
