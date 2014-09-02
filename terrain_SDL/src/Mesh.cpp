#include "Mesh.h"

// REFERENCE: ogldev.atspace.co.uk/www/tutorial22/tutorial22.html

Mesh::MeshUnit::MeshUnit()
{
    numFaces  = 0;
};

void Mesh::MeshUnit::initialize(const std::vector<Vertex>& Vertices)
{
    glGenBuffers(1, &VBO);
  	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);
}

Mesh::Mesh(){
	trimesh = new btTriangleMesh();
}

btCollisionShape* Mesh::getCollisionShape(){
	return shape;
}

void Mesh::clear()
{
	if(trimesh != NULL){
		delete trimesh;
	}
	trimesh = new btTriangleMesh();

    for (unsigned int i = 0 ; i < textures.size() ; i++) {
        delete(textures[i]);
    }
}


bool Mesh::loadMesh(const char* fileName){
    // delete previous mesh
    clear();

	bool success = false;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate);

	// check to see if scene loaded properly
	if (scene) {
        success = loadMeshesFromScene(scene, fileName);
    }
    else {
        printf("Error parsing '%s': '%s'\n", fileName, importer.GetErrorString());
    }

    return success;
}

bool Mesh::loadMeshesFromScene(const aiScene* scene, const char* fileName){
	// initialize size of mesh and texture arrays
	meshes.resize(scene->mNumMeshes);
	textures.resize(scene->mNumMaterials);

	// get vertices and textures for each mesh in the scene
	for(unsigned int i=0; i< meshes.size(); i++){
		const aiMesh* mesh = scene->mMeshes[i];
		initializeMesh(i, mesh);
	}

	shape = new btConvexTriangleMeshShape(trimesh);

	return initializeMaterials(scene, fileName);
}

void Mesh::initializeMesh(unsigned int index, const aiMesh* mesh){
	// initialize variables
	std::vector<Vertex> geometry;
	Vertex vert;
	aiVector3D texture;
	aiVector3D dfltVec(0.0f, 0.0f, 0.0f);
	
	meshes[index].numFaces = mesh->mNumFaces;
	meshes[index].materialIndex = mesh->mMaterialIndex;

	// get a face from the mesh
	for(unsigned int i = 0; i < mesh->mNumVertices; i++){
		const aiVector3D* pos = &(mesh->mVertices[i]);
		const aiVector3D* normal = &(mesh->mNormals[i]);
        const aiVector3D* tex = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &dfltVec;

		vert.position[0] = pos->x;
		vert.position[1] = pos->y;
		vert.position[2] = pos->z;
		vert.normal[0] = normal->x;
		vert.normal[1] = normal->y;
		vert.normal[2] = normal->z;
		vert.texture[0] = tex->x;
		vert.texture[1] = tex->y;

		// add vertex to geometry
		geometry.push_back(vert);
	}

	// add triangles to triangle mesh
	for(unsigned int i=0; i<geometry.size(); i++){
		trimesh->addTriangle(convertFArrayTobtVector(geometry[i].position), convertFArrayTobtVector(geometry[i+1].position), convertFArrayTobtVector(geometry[i+2].position));
		i+=2;
	}
	meshes[index].initialize(geometry);
}

bool Mesh::initializeMaterials(const aiScene* scene, const char* fileName){
	// convert fileName to string
	std::stringstream ss;
	std::string fileString;
	ss << fileName;
	ss >> fileString;

    // Extract the directory part from the file name
    std::string::size_type SlashIndex = fileString.find_last_of("/");
    std::string Dir;

    if (SlashIndex == std::string::npos) {
        Dir = ".";
    }
    else if (SlashIndex == 0) {
        Dir = "/";
    }
    else {
        Dir = fileString.substr(0, SlashIndex);
    }

    bool success = true;
	// initialize the materials
	for(unsigned int i=0; i < scene->mNumMaterials; i++){
		const aiMaterial* material = scene->mMaterials[i];
		
		textures[i] = NULL;
		if(material->GetTextureCount(aiTextureType_DIFFUSE) > 0){
			aiString filePath;
			
			if(material->GetTexture(aiTextureType_DIFFUSE, 0, &filePath, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS){
				std::string fullPath = Dir + "/" + filePath.data;
				textures[i] = new Texture(GL_TEXTURE_2D, fullPath.c_str());
				
				if(!textures[i]->Create()){
					//printf("Error loading the texture '%s'\n", fullPath.c_str());
					delete textures[i];
					textures[i] = NULL;
					success = false;
				}
	            else {
                    //printf("Loaded texture '%s'\n", fullPath.c_str());
                }			
			}
		}
	}

	return success;
}

void Mesh::renderMesh(GLint loc_position, GLint loc_normal)
{
    glEnableVertexAttribArray(loc_position);
    glEnableVertexAttribArray(loc_normal);

    for (unsigned int i = 0; i < meshes.size(); i++) {
        glBindBuffer(GL_ARRAY_BUFFER, meshes[i].VBO);

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

        const unsigned int materialIndex = meshes[i].materialIndex;

        if (materialIndex < textures.size() && textures[materialIndex]) {
            textures[materialIndex]->Bind(GL_TEXTURE0);
        }

        glDrawArrays(GL_TRIANGLES, 0, meshes[i].numFaces*3);
    }

    glDisableVertexAttribArray(loc_position);
    glDisableVertexAttribArray(loc_normal);

}

btVector3 Mesh::convertFArrayTobtVector(float arr[]){
	btVector3 vert;
	// convert float[3] to vert
	for(int i=0; i<3; i++){
		vert[0] = arr[0];
		vert[1] = arr[1];
		vert[2] = arr[2];
	}
	return vert;
}





