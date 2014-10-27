#include "Terrain.h"
#include <iostream>

Terrain::Terrain(GLint program, const char* fileName){
	this->program = program;
	gdalDataZone = NULL;
	dataZoneDataImage = NULL;
	gdalImage = NULL;
	geot = NULL;
	scale = 1;
	heightScale = 1;
	currentTime = 0;

    loc_position = glGetAttribLocation(program,
                    const_cast<const char*>("v_position"));
    if(loc_position == -1)
    {
        std::cerr << "[F] POSITION NOT FOUND" << std::endl;
        //return false;
    }

    loc_texture = glGetAttribLocation(program,
                    const_cast<const char*>("v_texture"));
    if(loc_texture == -1)
    {
        std::cerr << "[F] TEXTURE NOT FOUND" << std::endl;
        //return false;
    }

    loc_sampler = glGetUniformLocation(program,
                    const_cast<const char*>("gSampler"));
    if(loc_sampler == -1)
    {
        std::cerr << "[F] SAMPLER NOT FOUND" << std::endl;
        //return false;
    }

    loc_scalar = glGetUniformLocation(program,
                    const_cast<const char*>("verticalScalar"));
    if(loc_scalar == -1)
    {
        std::cerr << "[F] VERT SCALAR NOT FOUND" << std::endl;
        //return false;
    }

    loc_mvpmat = glGetUniformLocation(program,
                    const_cast<const char*>("mvpMatrix"));
    if(loc_mvpmat == -1)
    {
        std::cerr << "[F] MVP NOT FOUND" << std::endl;
        //return false;
    }

    // load terrain image / gdal data set
    heightMapImage = new Texture(GL_TEXTURE_2D, fileName);
	loaded = heightMapImage->create();

    int nBlockXSize, nBlockYSize;
    int bGotMin, bGotMax;
    double adfMinMax[2];

	// get height and width of text from dgal
	gdalImage = heightMapImage->getGDALImage();

	// get geo transform for dem
	// 0 = xpos, 1 = x meters, 2 = x rotation, 3 = yypos (negative), 4 = y rotation, 5 = y meters
	geot = new double[6];
	gdalImage->GetGeoTransform(geot);
	for(int i=0; i<6; i++){
		 //printf( "goi%i=%.3fd\n", i, geot[i] );
	}

	// get data from the raster
    poBand = gdalImage->GetRasterBand( 1 );
	bandXSize = poBand->GetXSize();

    poBand->GetBlockSize( &nBlockXSize, &nBlockYSize );
    /*printf( "Block=%dx%d Type=%s, ColorInterp=%s\n",
            nBlockXSize, nBlockYSize,
            GDALGetDataTypeName(poBand->GetRasterDataType()),
            GDALGetColorInterpretationName(
                poBand->GetColorInterpretation()) );*/

    adfMinMax[0] = poBand->GetMinimum( &bGotMin );
    adfMinMax[1] = poBand->GetMaximum( &bGotMax );
    if( ! (bGotMin && bGotMax) )
        GDALComputeRasterMinMax((GDALRasterBandH)poBand, TRUE, adfMinMax);

    printf( "Min=%.3fd, Max=%.3f\n", adfMinMax[0], adfMinMax[1] );
    
    if( poBand->GetOverviewCount() > 0 )
        printf( "Band has %d overviews.\n", poBand->GetOverviewCount() );

    if( poBand->GetColorTable() != NULL )
        printf( "Band has a color table with %d entries.\n", 
                 poBand->GetColorTable()->GetColorEntryCount() );

	// get min and max height of terrain
	min = adfMinMax[0];
	max = adfMinMax[1];
	range = max - min;
}

Terrain::~Terrain(){
	delete heightMapImage;

	//if(dataZoneDataImage != NULL)
		//delete dataZoneDataImage;

    GDALClose( (GDALDatasetH) gdalDataZone );  
}

void Terrain::setScale(float heightScale, float scale){
	this->scale = scale;
	this->heightScale = heightScale;	
}

int Terrain::getWidth(){
	return width;
}

int Terrain::getHeight(){
	return height;
}

void Terrain::setDataZone(const char* fileName, GLint program){
	// intialize and set datazone program
	dataZoneProgram = program;

    dz_loc_position = glGetAttribLocation(program,
                    const_cast<const char*>("v_position"));
    if(dz_loc_position == -1)
    {
        std::cerr << "[F] DATA ZONE POSITION NOT FOUND" << std::endl;
        //return false;
    }

    dz_loc_samplepos = glGetAttribLocation(program,
                    const_cast<const char*>("samplePos"));
    if(dz_loc_samplepos == -1)
    {
        std::cerr << "[F] DATA ZONE SAMPLE POSITION NOT FOUND" << std::endl;
        //return false;
    }

    dz_loc_sampler = glGetUniformLocation(program,
                    const_cast<const char*>("gSampler"));
    if(dz_loc_sampler == -1)
    {
        std::cerr << "[F] DATA ZONE SAMPLER NOT FOUND" << std::endl;
        //return false;
    }

    dz_loc_scalar = glGetUniformLocation(program,
                    const_cast<const char*>("verticalScalar"));
    if(dz_loc_scalar == -1)
    {
        std::cerr << "[F] DATA ZONE VERT SCALAR NOT FOUND" << std::endl;
        //return false;
    }

    dz_loc_mvpmat = glGetUniformLocation(program,
                    const_cast<const char*>("mvpMatrix"));
    if(dz_loc_mvpmat == -1)
    {
        std::cerr << "[F] DATA ZONE MVP NOT FOUND" << std::endl;
        //return false;
    }

	Texture* dataZone = new Texture(GL_TEXTURE_2D, fileName);
	dataZone->load();
	gdalDataZone = dataZone->getGDALImage();
}

bool Terrain::setDataZoneData(const char* fileName, bool persistantBand){
	// set the current data file name
	parseDataFileName(fileName);

    // delete old data zone data if it was set
	if(dataZoneDataImage != NULL){
		//delete dataZoneDataImage;
		//dataZoneDataImage = NULL;
	}

	Texture* temp = dataZoneDataImage;
	dataZoneDataImage = new Texture(GL_TEXTURE_2D, fileName);
	if(dataZoneDataImage->load()){
	    // set current data raster band and raster count
	    GDALDataset* dataset;
		dataset = dataZoneDataImage->getGDALImage();

		// if persistant band is not set, reset the band to 1, else keep it the same
		if(!persistantBand)
	    	currentDataBand = 1;

	    numDataBands = dataset->GetRasterCount();
		if(setDataZoneBand(currentDataBand))
			return true;
		else
			return false;
	}
	else{
		dataZoneDataImage = temp;
		return false;
	}

}

bool Terrain::setDataZoneBand(int num){
	if(gdalDataZone != NULL){

		// make sure the passed in band is within range	
		if(num < 1 || num > numDataBands){
			return false;
		}
		// load and set data
		GDALDataset* dataset;
	    int bGotMin, bGotMax;
	    double adfMinMax[2], range;
	    float colorPos;
	    std::vector<float> intensityInZone;

		dataset = dataZoneDataImage->getGDALImage();

	    GDALRasterBand* poDataBand = dataset->GetRasterBand( num );

	    adfMinMax[0] = poDataBand->GetMinimum( &bGotMin );
	    adfMinMax[1] = poDataBand->GetMaximum( &bGotMax );
	    if( ! (bGotMin && bGotMax) )
	        GDALComputeRasterMinMax((GDALRasterBandH)poDataBand, TRUE, adfMinMax);
	    range = adfMinMax[1] - adfMinMax[0];

		float* poScanLine = (float *) CPLMalloc(sizeof(float)*bandXSize);
		float* poScanLine2 = (float *) CPLMalloc(sizeof(float)*bandXSize);
		float* dataScanLine = (float *) CPLMalloc(sizeof(float)*bandXSize);
		float* dataScanLine2 = (float *) CPLMalloc(sizeof(float)*bandXSize);

		for(int y=0; y<height-1; y++){
			// get to rows of pixel values
			poDataBand->RasterIO(GF_Read, 0, y, bandXSize, 1, poScanLine, bandXSize, 1, GDT_Float32, 0, 0);
			poDataBand->RasterIO(GF_Read, 0, y+1, bandXSize, 1, poScanLine2, bandXSize, 1, GDT_Float32, 0, 0);

			poBandDataZone->RasterIO(GF_Read, 0, y, bandXSize, 1, dataScanLine, bandXSize, 1, GDT_Float32, 0, 0);
			poBandDataZone->RasterIO(GF_Read, 0, y+1, bandXSize, 1, dataScanLine2, bandXSize, 1, GDT_Float32, 0, 0);

			for(int x=0; x<width-1; x++){
					// check to see if triangle is in datazone
					if(checkDataZone(dataScanLine[x], dataScanLine[x+1], dataScanLine2[x])){
						// push color values for the three verticies in the triangle
						colorPos = (poScanLine[x]-adfMinMax[0])/range;
						intensityInZone.push_back(colorPos);
						colorPos = (poScanLine[x+1]-adfMinMax[0])/range;
						intensityInZone.push_back(colorPos);
						colorPos = (poScanLine2[x]-adfMinMax[0])/range;
						intensityInZone.push_back(colorPos);
					}

					// check to see if triangle is in datazone
					if(checkDataZone(dataScanLine2[x], dataScanLine2[x+1], dataScanLine[x+1])){
						colorPos = (poScanLine2[x]-adfMinMax[0])/range;
						intensityInZone.push_back(colorPos);
						colorPos = (poScanLine2[x+1]-adfMinMax[0])/range;
						intensityInZone.push_back(colorPos);
						colorPos = (poScanLine[x+1]-adfMinMax[0])/range;
						intensityInZone.push_back(colorPos);
					}
		
			}
		}

		// set intensity to all the data zone vertices
		for(int i=0; i<dataZoneVertices.size(); i++){
			dataZoneVertices[i].samplePos = intensityInZone[i];
		}
		// change buffer data so that it draws changes
		glBindBuffer(GL_ARRAY_BUFFER, dzVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexColor) * dataZoneVertices.size(), &dataZoneVertices[0], GL_DYNAMIC_DRAW);

		// clean up
		CPLFree(poScanLine);
		CPLFree(poScanLine2);
		CPLFree(dataScanLine);
		CPLFree(dataScanLine2);
		return true;
	}
	else{
		return false;
	}
}

void Terrain::parseDataFileName(const char* fileName){
	std::string str(fileName);

	// get directory and beginning of name
	directoryAndName = str.substr(0, str.find("."));

	// get time number
	std::string s = str.substr(str.find(".")+1);
	std::string time = s.substr(0, str.find("."));
	currentTime = atoi(time.c_str());

	// get extension
	extension = s.substr(s.rfind(".")+1);
}

void Terrain::showNextDataTime(){
	if(gdalDataZone != NULL){
		std::string fileName(directoryAndName);
		currentTime++;
		// create file to load data from
		fileName.append(".");
		fileName.append(std::to_string(currentTime));
		fileName.append(".");
		fileName.append(extension);

		// load data zone data
		if(!setDataZoneData(fileName.c_str(), true)){
			currentTime--;
		}
	}
}
void Terrain::showPrevDataTime(){
	if(gdalDataZone != NULL){
		std::string fileName(directoryAndName);
		currentTime--;
		// create file to load data from
		fileName.append(".");
		fileName.append(std::to_string(currentTime));
		fileName.append(".");
		fileName.append(extension);

		// load data zone data
		if(!setDataZoneData(fileName.c_str(), true)){
			currentTime++;
		}
	}
}
void Terrain::showNextDataBand(){
	if(gdalDataZone != NULL){
		currentDataBand++;
		if(!setDataZoneBand(currentDataBand)){
			currentDataBand--;
		}
	}
}
void Terrain::showPrevDataBand(){
	if(gdalDataZone != NULL){
		currentDataBand--;
		if(!setDataZoneBand(currentDataBand)){
			currentDataBand++;
		}
	}
}

bool Terrain::generateMesh(){
	if(loaded){
		width = gdalImage->GetRasterXSize();
		height = gdalImage->GetRasterYSize();
		VertexTexture vert;
    	int bGotMin, bGotMax;
		VertexColor vertColor;
		int xOffset, yOffset;

		float* poScanLine = (float *) CPLMalloc(sizeof(float)*bandXSize);
		float* poScanLine2 = (float *) CPLMalloc(sizeof(float)*bandXSize);
		//std::cout << "xsize" << width << std::endl;

		// generate a planar triangle mesh that corresponds with the texture
		xOffset = width/2;
		yOffset = height/2;

		// setup datazone if one is set
		float* dataScanLine = (float *) CPLMalloc(sizeof(float)*bandXSize);
		float* dataScanLine2 = (float *) CPLMalloc(sizeof(float)*bandXSize);
		float dzMin, dzMax, dzRange;
		if(gdalDataZone != NULL){
			poBandDataZone = gdalDataZone->GetRasterBand(1);
			dataScanLine = (float *) CPLMalloc(sizeof(float)*bandXSize);
			dataScanLine2 = (float *) CPLMalloc(sizeof(float)*bandXSize);

			double dzAdfMinMax[2];
		    dzAdfMinMax[0] = poBand->GetMinimum( &bGotMin );
		    dzAdfMinMax[1] = poBand->GetMaximum( &bGotMax );
		    if( ! (bGotMin && bGotMax) )
		        GDALComputeRasterMinMax((GDALRasterBandH)poBandDataZone, TRUE, dzAdfMinMax);

		    //printf( "Min=%.3fd, Max=%.3f\n", dzAdfMinMax[0], dzAdfMinMax[1] );
		    dzMin = dzAdfMinMax[0];
		    dzMax = dzAdfMinMax[1];
		    dzRange = dzMax - dzMin;
		}

		for(int y=-yOffset; y<height-yOffset-1; y++){
			// get to rows of pixel values
			poBand->RasterIO(GF_Read, 0, y+yOffset, bandXSize, 1, poScanLine, bandXSize, 1, GDT_Float32, 0, 0);
			poBand->RasterIO(GF_Read, 0, y+yOffset+1, bandXSize, 1, poScanLine2, bandXSize, 1, GDT_Float32, 0, 0);

			if(gdalDataZone != NULL){
				poBandDataZone->RasterIO(GF_Read, 0, y+yOffset, bandXSize, 1, dataScanLine, bandXSize, 1, GDT_Float32, 0, 0);
				poBandDataZone->RasterIO(GF_Read, 0, y+yOffset+1, bandXSize, 1, dataScanLine2, bandXSize, 1, GDT_Float32, 0, 0);
			}

			for(int x=-xOffset; x<width-xOffset-1; x++){
					// check to see if triangle is in datazone
					if(gdalDataZone == NULL || !checkDataZone(dataScanLine[x+xOffset], dataScanLine[x+xOffset+1], dataScanLine2[x+xOffset])){
						// push top row of triangles
						vert.position[0] = x*scale;
						vert.position[1] = (poScanLine[x+xOffset]-min)/range;
						vert.position[2] = y*scale;
						vert.texture[0] = (float)(x+xOffset)/width;
						vert.texture[1] = (float)(y + yOffset)/height;
						planeVertices.push_back(vert);
						vert.position[0] = (x+1)*scale;
						vert.position[1] = (poScanLine[x+xOffset+1]-min)/range;
						vert.position[2] = y*scale;
						vert.texture[0] = (float)(x+1 + xOffset)/width;
						vert.texture[1] = (float)(y + yOffset)/height;
						planeVertices.push_back(vert);
						vert.position[0] = x*scale;
						vert.position[1] = (poScanLine2[x+xOffset]-min)/range;
						vert.position[2] = (y+1)*scale;	
						vert.texture[0] = (float)(x + xOffset)/width;
						vert.texture[1] = (float)(y+1 + yOffset)/height;
						planeVertices.push_back(vert);
					}
					else{
						vertColor.position[0] = x*scale;
						vertColor.position[1] = (poScanLine[x+xOffset]-min)/range;
						vertColor.position[2] = y*scale;
						vertColor.samplePos = (dataScanLine[x+xOffset]-dzMin)/dzRange;
						dataZoneVertices.push_back(vertColor);
						vertColor.position[0] = (x+1)*scale;
						vertColor.position[1] = (poScanLine[x+xOffset+1]-min)/range;
						vertColor.position[2] = y*scale;
						vertColor.samplePos = (dataScanLine[x+xOffset+1]-dzMin)/dzRange;
						dataZoneVertices.push_back(vertColor);
						vertColor.position[0] = x*scale;
						vertColor.position[1] = (poScanLine2[x+xOffset]-min)/range;
						vertColor.position[2] = (y+1)*scale;
						vertColor.samplePos = (dataScanLine2[x+xOffset]-dzMin)/dzRange;
						dataZoneVertices.push_back(vertColor);
					}

					// check to see if triangle is in datazone
					if(gdalDataZone == NULL || !checkDataZone(dataScanLine2[x+xOffset], dataScanLine2[x+xOffset+1], dataScanLine[x+xOffset+1])){
						// push vertices of first triangle
						vert.position[0] = x*scale;
						vert.position[1] = (poScanLine2[x+xOffset]-min)/range;
						vert.position[2] = (y+1)*scale;
						vert.texture[0] = (float)(x + xOffset)/width;
						vert.texture[1] = (float)(y+1 + yOffset)/height;
						planeVertices.push_back(vert);
						vert.position[0] = (x+1)*scale;
						vert.position[1] = (poScanLine2[x+1+xOffset]-min)/range;
						vert.position[2] = (y+1)*scale;
						vert.texture[0] = (float)(x+1 + xOffset)/width;
						vert.texture[1] = (float)(y+1 + yOffset)/height;
						planeVertices.push_back(vert);
						vert.position[0] = (x+1)*scale;
						vert.position[1] = (poScanLine[x+xOffset+1]-min)/range;
						vert.position[2] = y*scale;
						vert.texture[0] = (float)(x+1 + xOffset)/width;
						vert.texture[1] = (float)(y + yOffset)/height;
						planeVertices.push_back(vert);
					}
					else{
						// push vertices of first triangle
						vertColor.position[0] = x*scale;
						vertColor.position[1] = (poScanLine2[x+xOffset]-min)/range;
						vertColor.position[2] = (y+1)*scale;
						vertColor.samplePos = (dataScanLine2[x+xOffset]-dzMin)/dzRange;
						dataZoneVertices.push_back(vertColor);
						vertColor.position[0] = (x+1)*scale;
						vertColor.position[1] = (poScanLine2[x+1+xOffset]-min)/range;
						vertColor.position[2] = (y+1)*scale;
						vertColor.samplePos = (dataScanLine2[x+xOffset+1]-dzMin)/dzRange;
						dataZoneVertices.push_back(vertColor);
						vertColor.position[0] = (x+1)*scale;
						vertColor.position[1] = (poScanLine[x+xOffset+1]-min)/range;
						vertColor.position[2] = y*scale;
						vertColor.samplePos = (dataScanLine[x+xOffset+1]-dzMin)/dzRange;
						dataZoneVertices.push_back(vertColor);
					}
		
			}
		}

		for(int i=0; i<planeVertices.size(); i++){
			//if(i % 100)
			//std::cout << planeVertices[i].texture[0] << " x y " << planeVertices[i].texture[1] << std::endl;
		}

		// create vbo for terrain
	    glGenBuffers(1, &VBO);
	    glBindBuffer(GL_ARRAY_BUFFER, VBO);
	    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexTexture) * planeVertices.size(), &planeVertices[0], GL_STATIC_DRAW);

	    // create vbo for data zone
		if(gdalDataZone != NULL){
		    glGenBuffers(1, &dzVBO);
		    glBindBuffer(GL_ARRAY_BUFFER, dzVBO);
		    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexColor) * dataZoneVertices.size(), &dataZoneVertices[0], GL_STATIC_DRAW);
		}

		// clean up
		CPLFree(poScanLine);
		CPLFree(poScanLine2);
		CPLFree(dataScanLine);
		CPLFree(dataScanLine2);

		return true;
	}
	else{
		return false;
	}
}

void Terrain::cutSquareHole(float startX, float startZ, float xSize, float zSize){
	// loop through mesh and cut a hole in it
	Vertex vert1, vert2, vert3;
	vert1 = planeVertices[0];
	//std::cout << vert1.position[0] << ' ' << vert1.position[2] << std::endl;
	//std::cout << "sx: " << startX << " sy: " << startZ << " xs: " << xSize << " ys: " << zSize << std::endl;
			//std::cout << "total " << planeVertices.size() << std::endl;
	// loop through plane vertices
	float endX = startX + xSize, endZ = startZ + zSize;
	int removed = 0;
	for(int i=0; i<planeVertices.size(); i+=3){
		// grab 3 vertices at a time and remove them if in the square
		vert1 = planeVertices[i];
		vert2 = planeVertices[i+1];
		vert3 = planeVertices[i+2];
		if((vert1.position[0] > startX && vert1.position[0] < endX) && (vert1.position[2] > startZ && vert1.position[2] < endZ) &&
			(vert2.position[0] > startX && vert2.position[0] < endX) && (vert2.position[2] > startZ && vert2.position[2] < endZ) &&
			(vert3.position[0] > startX && vert3.position[0] < endX) && (vert3.position[2] > startZ && vert3.position[2] < endZ)){
			removed+=3;
			planeVertices.erase(planeVertices.begin()+i, planeVertices.begin()+i+2);
		}
	}
			std::cout << "removed " << removed << std::endl;

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * planeVertices.size(), &planeVertices[0], GL_STATIC_DRAW);
}

bool Terrain::checkDataZone(float v1, float v2, float v3){
	if(v1 > 0 && v2 > 0 && v3 > 0)
		return true;
	else
		return false;
}

void Terrain::geoTransform(GDALDataset* projDataSet, double& x, double& y){
	std::string proj;
	proj = std::string(projDataSet->GetProjectionRef());

	OGRSpatialReference sr;

	char* test = &proj[0];
	sr.importFromWkt(&test);

	proj = std::string(getGdalDataset()->GetProjectionRef());

	OGRSpatialReference sr2;
	test = &proj[0];
	sr2.importFromWkt(&test);

	OGRCoordinateTransformation* poTransform = OGRCreateCoordinateTransformation(&sr, &sr2);
	double* geot = new double[6];
	projDataSet->GetGeoTransform(geot);
	double oldX = geot[0];
	double oldY = geot[3];
	poTransform->Transform(1, &oldX, &oldY);
	x = oldX;
	y = oldY;
}

void Terrain::buttonPressed(){
	exit(1);
}

void Terrain::registerForEvents(){
	//buttonWindow->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(Terrain::buttonPressed, this));
}

void Terrain::renderDataZone(glm::mat4 projection, glm::mat4 view){
    //premultiply the matrix for this example
    glm::mat4 mvp = projection * view * model;

    // enable the shader program
    glUseProgram(dataZoneProgram);

    glBindBuffer(GL_ARRAY_BUFFER, dzVBO);

    glEnableVertexAttribArray(dz_loc_position);
    glEnableVertexAttribArray(dz_loc_samplepos);

    //upload the matrix to the shader
    glUniformMatrix4fv(dz_loc_mvpmat, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniform1i(dz_loc_sampler, 0);
    glUniform1f(dz_loc_scalar, heightScale);

	glVertexAttribPointer( dz_loc_position,//location of attribute
	                       3,//number of elements
	                       GL_FLOAT,//type
	                       GL_FALSE,//normalized?
	                       sizeof(VertexColor),//stride
	                       0);//offset

	glVertexAttribPointer( dz_loc_samplepos,//location of attribute
	                       1,//number of elements
	                       GL_FLOAT,//type
	                       GL_FALSE,//normalized?
	                       sizeof(VertexColor),//stride
	                       (void*) (3*sizeof(float)));//offset

	dataTexture->bind(GL_TEXTURE0);

    glDrawArrays(GL_TRIANGLES, 0, dataZoneVertices.size());
    
    glDisableVertexAttribArray(dz_loc_position);
    glDisableVertexAttribArray(dz_loc_samplepos);
    glUseProgram(0);
}

void Terrain::render(glm::mat4 projection, glm::mat4 view){
	//model = glm::translate( glm::mat4(1.0f), glm::vec3(0, 50, 0));
    //premultiply the matrix for this example
    glm::mat4 mvp = projection * view * model;

    // enable the shader program
    glUseProgram(program);

    glEnableVertexAttribArray(loc_position);
    glEnableVertexAttribArray(loc_texture);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //upload the matrix to the shader
    glUniformMatrix4fv(loc_mvpmat, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniform1i(loc_sampler, 0);
    glUniform1f(loc_scalar, heightScale);

	glVertexAttribPointer( loc_position,//location of attribute
	                       3,//number of elements
	                       GL_FLOAT,//type
	                       GL_FALSE,//normalized?
	                       sizeof(VertexTexture),//stride
	                       0);//offset

	glVertexAttribPointer( loc_texture,//location of attribute
	                       2,//number of elements
	                       GL_FLOAT,//type
	                       GL_FALSE,//normalized?
	                       sizeof(VertexTexture),//stride
	                       (void*) (3*sizeof(float)));//offset

	heightMapImage->bind(GL_TEXTURE0);

    glDrawArrays(GL_TRIANGLES, 0, planeVertices.size());

    glDisableVertexAttribArray(loc_position);
    glDisableVertexAttribArray(loc_texture);
    glUseProgram(0);

    // render datazones if there are any
    if(gdalDataZone != NULL){
    	renderDataZone(projection, view);
    }

    //renderShapes(projection, view);
}

void Terrain::renderShapes(glm::mat4 projection, glm::mat4 view){
	for(Shape* shape: shapes){
		shape->render(projection, view);
	}
}

GDALDataset* Terrain::getGdalDataset(){
	return heightMapImage->getGDALImage();
}

double* Terrain::getGeot(){
	return geot;
}

const std::vector<VertexTexture>& Terrain::getVertices(){
	return planeVertices;
}

void Terrain::relativeTranslate(glm::vec3 vec3){
	model = glm::translate( model, vec3);
}

void Terrain::setMinMax(float min, float max){
	this->min = min;
	this->max = max;
	this->range = max - min;
}

float Terrain::getMin(){
	return min;
}

float Terrain::getMax(){
	return max;
}

void Terrain::addShape(const char* fileName, GLint prog, glm::vec3 color, float isBoundary){
   	Shape* shape = new Shape();
    shape->setOrigin(geot[0] + (width/2)*geot[1], geot[3] - (height/2)*geot[1]);
    shape->setScale(1/geot[1] * scale, (float) heightScale);
    //std::cout << getWidth() << " w : h " << getHeight();
    shape->setProgram(prog);
    shape->setColor(color);
    shape->init(fileName, isBoundary);	
    shapes.push_back(shape);
}

void Terrain::placeShapesOnSurface(){
	float** data = new float*[height];
	float heightOffset = .08f * scale;
	double xOrigin = planeVertices[0].position[0], yOrigin = planeVertices[0].position[2];
	int x,y;

	for(int i=0; i<height; i++){
		data[i] = new float[width];
	}
	GDALRasterBand* poDataBand = gdalImage->GetRasterBand( 1 );

	// load all of the images height values into the data array
	for(int i=0; i<height; i++){
		poDataBand->RasterIO(GF_Read, 0, i, bandXSize, 1, data[i], bandXSize, 1, GDT_Float32, 0, 0);
	}

	for(Shape* shape: shapes){
		// loop through each vertex in the shape file and set its height to the corresponding terrain height
		std::vector<Vertex> shapeVertices = shape->getShapeVertices();
		double scale = shape->getScale();
		for(int i=0; i<shapeVertices.size(); i++){
			x = (shapeVertices[i].position[0] - xOrigin)/(scale*geot[1]);
			y = (shapeVertices[i].position[2] - yOrigin)/(scale*geot[1]);
			shapeVertices[i].position[1] = (data[y][x]-min)/range + heightOffset;
		}

		// set shape vertices to new data
		shape->setShapeVertices(shapeVertices);		
	}


	// delete data
	for(int i=0; i<height; i++)
		delete data[i];
	delete data;
}

void Terrain::setMaskTexture(Texture* tex){
	dataTexture = tex;
}

void Terrain::useGrayScaleTexture(){
	float** data = new float*[height];
    std::default_random_engine rand;
    std::uniform_int_distribution<int> distr(-3, 3);
	int baseRed = 50, baseGreen = 70, baseBlue = 15;
	float stepRed = (float)(200 - baseRed)*.8, stepGreen = (float)(200 - baseGreen)*.8, stepBlue = (float)(200 - baseBlue)*.8;
	float step;
	
	for(int i=0; i<height; i++){
		data[i] = new float[width];
	}
	GDALRasterBand* poDataBand = gdalImage->GetRasterBand( 1 );

	// load all of the images height values into the data array
	for(int i=0; i<height; i++){
		poDataBand->RasterIO(GF_Read, 0, i, bandXSize, 1, data[i], bandXSize, 1, GDT_Float32, 0, 0);
	}

	//std::cout << heightMapImage->getHeight() << ' ' << width << std::endl;
	//std::cout << heightMapImage->getWidth() * heightMapImage->getHeight() << std::endl;
	for(int i=0; i<heightMapImage->getHeight(); i++){
		for(int j=0; j<heightMapImage->getWidth(); j++){
			step = (data[i][j]-min)/range;
			// brown is #5C3317
			heightMapImage->setPixelColor(j, i, glm::vec3(baseRed + distr(rand) + step*stepRed,
			 baseGreen + distr(rand) + step*stepGreen, baseBlue + distr(rand) + step*stepBlue));
		}
	}
	heightMapImage->bind(GL_TEXTURE0);
	heightMapImage->setTextureFromArrayInfo();

	// delete data
	for(int i=0; i<height; i++)
		delete data[i];
	delete data;

}

void Terrain::colorSurfaceWithShapes(){
	float angle, dist;
 	Vertex v1, v2;
	double xOrigin = planeVertices[0].position[0], yOrigin = planeVertices[0].position[2];

	bool thick = true;

	for(Shape* shape: shapes){
		// loop through each vertex in the shape file and set its height to the corresponding terrain height
		std::vector<Vertex> shapeVertices = shape->getShapeVertices();
		std::vector<int> sizes = shape->getLineSizes();
		int pos = 0;
		double scale = shape->getScale();
		glm::vec3 color = shape->getColor();
		color[0] *= 255;
		color[1] *= 255;
		color[2] *= 255;

		for(int s = 0; s < sizes.size(); s++){
			//std::cout << sizes[s] << " s p " << pos << std::endl;
			for(int i=0; i < sizes[s] -1; i++){
				// get the 4 vertices of the rectangle
				v1 = shapeVertices[pos];
				v2 = shapeVertices[pos+1];

	    		angle = atan2(v2.position[2] - v1.position[2], v2.position[0] - v1.position[0]);
	    		if(angle < 0){angle += 3.14*2; }


	    		float x1 = (v1.position[0]- xOrigin)/(scale*geot[1]);
	    		float y1 = (v1.position[2]- yOrigin)/(scale*geot[1]);
	    		float x2 = (v2.position[0]- xOrigin)/(scale*geot[1]);
	    		float y2 = (v2.position[2]- yOrigin)/(scale*geot[1]);

	    		dist = sqrt(pow((x2 - x1),2) + pow((y2 - y1),2));

	    		float dx = cos(angle);
	    		float dy = sin(angle);


				for(float x = x1, y = y1, d = 0; d < dist; x += dx, y+= dy, d++){
					heightMapImage->setPixelColor((int)round(x), (int)round(y), color);
					if(thick){
						heightMapImage->setPixelColor((int)round(x)+1, (int)round(y), color);
						heightMapImage->setPixelColor((int)round(x), (int)round(y)+1, color);
					}
				}	

				if(i == sizes[s] -2){
					pos++;
				}
				pos++;
			}
		}
	
	}
	heightMapImage->bind(GL_TEXTURE0);
	heightMapImage->setTextureFromArrayInfo();	
}
