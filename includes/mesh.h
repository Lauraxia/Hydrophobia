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

class mesh
{
public:
	// the global Assimp scene object
	const aiScene* scene;
	float scale;
	std::map<std::string, GLuint*> textureIdMap;	// map image filenames to textureIds
	GLuint*		textureIds;							// pointer to texture Array
public:
	mesh();
	mesh::mesh(const std::string& filename);
	~mesh();
public:
	void render ();
	bool loadasset (const char* path);
	bool mesh::loadasset( const std::string& pFile);
private:
	void apply_material(const struct aiMaterial *mtl);
	void recursive_render (const struct aiScene *sc, const struct aiNode* nd, float pScale);
	bool loadtexture();
};