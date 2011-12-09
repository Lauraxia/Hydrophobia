// assimp include files. These three are usually needed.
#include <map>
#include <GL\glew.h>
#include "includes\common.h"

#include <vector>
#include <Assimp\assimp.hpp>	
#include <assimp\assimp.h>
#include <Assimp\aiPostProcess.h>
#include <Assimp\aiScene.h>

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

// Information to render each assimp node
//struct myMesh{
//
//	GLuint vao;
//	GLuint texIndex;
//	GLuint uniformBlockIndex;
//	int numFaces;
//};

class mesh
{
public:
	// the global Assimp scene object
	const aiScene* scene;

	// scale factor for the model to fit in the window
	//float scaleFactor;
	// images / texture
	// map image filenames to textureIds
	// pointer to texture Array
	//std::map<std::string, GLuint> textureIdMap;	
	//std::vector<struct MyMesh> myMeshes;
public:
	mesh();
	~mesh();
public:
	void render (const struct aiScene *sc, const struct aiNode* nd);
	//bool Import3DFromFile( const std::string& pFile);
	bool loadasset (const char* path);
	void apply_material(const struct aiMaterial *mtl);
private:
	//void init(const aiScene* pScene, const std::string& Filename);
	//void initMesh(unsigned int Index, const aiMesh* paiMesh);
	//int LoadGLTextures(const aiScene* scene);
	//void get_bounding_box_for_node (const struct aiNode* nd, struct aiVector3D* min, struct aiVector3D* max);
	//void get_bounding_box (struct aiVector3D* min, struct aiVector3D* max);
};