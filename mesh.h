// assimp include files. These three are usually needed.

#include "includes\common.h"

#include <Assimp\assimp.hpp>	
#include <assimp\assimp.h>
#include <Assimp\aiPostProcess.h>
#include <Assimp\aiScene.h>
#include <map>
#include <vector>

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
	float scale;

	std::map<std::string, GLuint*> textureIdMap;	// map image filenames to textureIds
	GLuint*		textureIds;							// pointer to texture Array
	// scale factor for the model to fit in the window
	//float scaleFactor;
	// images / texture
	// map image filenames to textureIds
	// pointer to texture Array
	//std::map<std::string, GLuint> textureIdMap;	
	//std::vector<struct MyMesh> myMeshes;
public:
	mesh();
	mesh::mesh(const std::string& filename);
	~mesh();
public:
	void render ();
	//bool Import3DFromFile( const std::string& pFile);
	bool loadasset (const char* path);
	bool mesh::loadasset( const std::string& pFile);
	void apply_material(const struct aiMaterial *mtl);
private:
	void recursive_render (const struct aiScene *sc, const struct aiNode* nd, float pScale);
	bool loadtexture();
	//void init(const aiScene* pScene, const std::string& Filename);
	//void initMesh(unsigned int Index, const aiMesh* paiMesh);
	//int LoadGLTextures(const aiScene* scene);
	//void get_bounding_box_for_node (const struct aiNode* nd, struct aiVector3D* min, struct aiVector3D* max);
	//void get_bounding_box (struct aiVector3D* min, struct aiVector3D* max);
};