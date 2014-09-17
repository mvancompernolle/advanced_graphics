#include "Terrain.h"
#include <iostream>

Terrain::Terrain(GLint program){
	this->program = program;
	gdalDataZone = NULL;
	dataZoneDataImage = NULL;
	currentTime = 0;

    loc_position = glGetAttribLocation(program,
                    const_cast<const char*>("v_position"));
    if(loc_position == -1)
    {
        std::cerr << "[F] POSITION NOT FOUND" << std::endl;
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
}

Terrain::~Terrain(){
	delete heightMapImage;

	//if(dataZoneDataImage != NULL)
		//delete dataZoneDataImage;

    GDALClose( (GDALDatasetH) gdalDataZone );  
}

bool Terrain::loadHeightMap(const char* fileName, float heightScale, float scale){
	// get terrain texture
	this->scale = scale;
	this->heightScale = heightScale;
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
		GDALRasterBand  *poBand;
		GDALDataset* dataset;
	    int bGotMin, bGotMax;
	    double adfMinMax[2], range;
	    float colorPos;
	    std::vector<float> intensityInZone;

		dataset = dataZoneDataImage->getGDALImage();

	    poBand = dataset->GetRasterBand( num );

	    adfMinMax[0] = poBand->GetMinimum( &bGotMin );
	    adfMinMax[1] = poBand->GetMaximum( &bGotMax );
	    if( ! (bGotMin && bGotMax) )
	        GDALComputeRasterMinMax((GDALRasterBandH)poBand, TRUE, adfMinMax);
	    range = adfMinMax[1] - adfMinMax[0];

		float* poScanLine = (float *) CPLMalloc(sizeof(float)*bandXSize);
		float* poScanLine2 = (float *) CPLMalloc(sizeof(float)*bandXSize);
		float* dataScanLine = (float *) CPLMalloc(sizeof(float)*bandXSize);
		float* dataScanLine2 = (float *) CPLMalloc(sizeof(float)*bandXSize);

		for(int y=0; y<height-1; y++){
			// get to rows of pixel values
			poBand->RasterIO(GF_Read, 0, y, bandXSize, 1, poScanLine, bandXSize, 1, GDT_Float32, 0, 0);
			poBand->RasterIO(GF_Read, 0, y+1, bandXSize, 1, poScanLine2, bandXSize, 1, GDT_Float32, 0, 0);

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
	// get height and width of text from dgal
	GDALDataset* gdalImage = heightMapImage->getGDALImage();

	width = gdalImage->GetRasterXSize();
	height = gdalImage->GetRasterYSize();
	Vertex vert;
	VertexColor vertColor;
	int xOffset, yOffset;

	// get data from the raster
	GDALRasterBand  *poBand;
    poBand = gdalImage->GetRasterBand( 1 );
	//int rasterCount = gdalImage->GetRasterCount();
	bandXSize = poBand->GetXSize();
    int nBlockXSize, nBlockYSize;
    int bGotMin, bGotMax;
    double adfMinMax[2];
    
    poBand->GetBlockSize( &nBlockXSize, &nBlockYSize );
    printf( "Block=%dx%d Type=%s, ColorInterp=%s\n",
            nBlockXSize, nBlockYSize,
            GDALGetDataTypeName(poBand->GetRasterDataType()),
            GDALGetColorInterpretationName(
                poBand->GetColorInterpretation()) );

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
	int min = adfMinMax[0];
	int max = adfMinMax[1];
	int range = max - min;

	float* poScanLine = (float *) CPLMalloc(sizeof(float)*bandXSize);
	float* poScanLine2 = (float *) CPLMalloc(sizeof(float)*bandXSize);

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

	    printf( "Min=%.3fd, Max=%.3f\n", dzAdfMinMax[0], dzAdfMinMax[1] );
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
					planeVertices.push_back(vert);
					vert.position[0] = (x+1)*scale;
					vert.position[1] = (poScanLine[x+xOffset+1]-min)/range;
					vert.position[2] = y*scale;
					planeVertices.push_back(vert);
					vert.position[0] = x*scale;
					vert.position[1] = (poScanLine2[x+xOffset]-min)/range;
					vert.position[2] = (y+1)*scale;	
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
					planeVertices.push_back(vert);
					vert.position[0] = (x+1)*scale;
					vert.position[1] = (poScanLine2[x+1+xOffset]-min)/range;
					vert.position[2] = (y+1)*scale;
					planeVertices.push_back(vert);
					vert.position[0] = (x+1)*scale;
					vert.position[1] = (poScanLine[x+xOffset+1]-min)/range;
					vert.position[2] = y*scale;
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

	// create vbo for terrain
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * planeVertices.size(), &planeVertices[0], GL_STATIC_DRAW);

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

bool Terrain::checkDataZone(float v1, float v2, float v3){
	if(v1 > 0 && v2 > 0 && v3 > 0)
		return true;
	else
		return false;
}

void Terrain::geoTransform(GDALDataset* projDataSet){
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
	double x = geot[0];
	double y = geot[3];
 printf( "x=%.3fd, y=%.3f\n", x, y );
	poTransform->Transform(1, &x, &y);
 printf( "x=%.3fd, y=%.3f\n", x, y );
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
	                       (void*)offsetof(VertexColor,samplePos));//offset

    glDrawArrays(GL_TRIANGLES, 0, dataZoneVertices.size());
    
    glDisableVertexAttribArray(dz_loc_position);
    glDisableVertexAttribArray(dz_loc_samplepos);
    glUseProgram(0);
}

void Terrain::render(glm::mat4 projection, glm::mat4 view){
    //premultiply the matrix for this example
    glm::mat4 mvp = projection * view * model;

    // enable the shader program
    glUseProgram(program);

    glEnableVertexAttribArray(loc_position);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //upload the matrix to the shader
    glUniformMatrix4fv(loc_mvpmat, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniform1i(loc_sampler, 0);
    glUniform1f(loc_scalar, heightScale);

	glVertexAttribPointer( loc_position,//location of attribute
	                       3,//number of elements
	                       GL_FLOAT,//type
	                       GL_FALSE,//normalized?
	                       sizeof(Vertex),//stride
	                       0);//offset

    glDrawArrays(GL_TRIANGLES, 0, planeVertices.size());
    
    glDisableVertexAttribArray(loc_position);
    glUseProgram(0);

    // render datazones if there are any
    if(gdalDataZone != NULL){
    	renderDataZone(projection, view);
    }
}

GDALDataset* Terrain::getGdalDataset(){
	return heightMapImage->getGDALImage();
}
