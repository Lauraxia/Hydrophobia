// Mesh datatype using Open Asset Import Library to store, and 
// render 3D models
// This code is based on the sample code included with the Library

#include <fstream>
#include <IL\il.h>
#include <string>
#include <map>
#include "mesh.h"


GLboolean abortGLInit(const char*);


// Create an instance of the Importer class
Assimp::Importer importer;

// Helpful functions for mesh

// Can't send color down as a pointer to aiColor4D because AI colors are ABGR.
void Color4f(const struct aiColor4D *color)
{
	glColor4f(color->r, color->g, color->b, color->a);
}
// ----------------------------------------------------------------------------

void color4_to_float4(const struct aiColor4D *c, float f[4])
{
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}
// ----------------------------------------------------------------------------

void set_float4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}

// ----------------------------------------------------------------------------
mesh::mesh()
{
	scale = 1;
	scene = NULL;
	//scaleFactor = 1;
	//loadasset(path);
	//LoadGLTextures(scene);
}
mesh::mesh(const std::string& path)
{
	scale = 0.3;
	loadasset(path);
	loadtexture();
}
mesh::~mesh()
{
	// cleaning up


	// cleanup - calling 'aiReleaseImport' is important, as the library 
	// keeps internal resources until the scene is freed again. Not 
	// doing so can cause severe resource leaking.
	aiReleaseImport(scene);
	//textureIdMap.clear(); 
}
//------------------------------------------------------------------------------
void mesh::render ()
{
	recursive_render(scene, scene->mRootNode,scale);
}

//------------------------------------------------------------------------------
void mesh::recursive_render (const struct aiScene *sc, const struct aiNode* nd, float scale)
{
	unsigned int i;
	unsigned int n=0, t;
	struct aiMatrix4x4 m = nd->mTransformation;

	m.Scaling(aiVector3D(scale, scale, scale), m);

	// update transform
	m.Transpose();
	glPushMatrix();
	glMultMatrixf((float*)&m);

	// draw all meshes assigned to this node
	for (; n < nd->mNumMeshes; ++n)
	{
		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];

		apply_material(sc->mMaterials[mesh->mMaterialIndex]);


		if(mesh->mNormals == NULL)
		{
			glDisable(GL_LIGHTING);
		}
		else
		{
			glEnable(GL_LIGHTING);
		}

		if(mesh->mColors[0] != NULL)
		{
			glEnable(GL_COLOR_MATERIAL);
		}
		else
		{
			glDisable(GL_COLOR_MATERIAL);
		}



		for (t = 0; t < mesh->mNumFaces; ++t) {
			const struct aiFace* face = &mesh->mFaces[t];
			GLenum face_mode;

			switch(face->mNumIndices)
			{
				case 1: face_mode = GL_POINTS; break;
				case 2: face_mode = GL_LINES; break;
				case 3: face_mode = GL_TRIANGLES; break;
				default: face_mode = GL_POLYGON; break;
			}

			glBegin(face_mode);

			for(i = 0; i < face->mNumIndices; i++)		// go through all vertices in face
			{
				int vertexIndex = face->mIndices[i];	// get group index for current index
				if(mesh->mColors[0] != NULL)
					Color4f(&mesh->mColors[0][vertexIndex]);
				if(mesh->mNormals != NULL)

					if(mesh->HasTextureCoords(0))		//HasTextureCoords(texture_coordinates_set)
					{
						glTexCoord2f(mesh->mTextureCoords[0][vertexIndex].x, 1 - mesh->mTextureCoords[0][vertexIndex].y); //mTextureCoords[channel][vertex]
					}

					glNormal3fv(&mesh->mNormals[vertexIndex].x);
					glVertex3fv(&mesh->mVertices[vertexIndex].x);
			}

			glEnd();

		}

	}


	// draw all children
	for (n = 0; n < nd->mNumChildren; ++n)
	{
		recursive_render(sc, nd->mChildren[n], scale);
	}

	glPopMatrix();
}
GLboolean abortGLInit(const char* abortMessage)
{
	//KillGLWindow();									// Reset Display
	MessageBox(NULL, abortMessage, "ERROR", MB_OK|MB_ICONEXCLAMATION);
	return FALSE;									// quit and return False
}
//------------------------------------------------------------------------------
//void mesh::recursive_render (const struct aiScene *sc, const struct aiNode* nd)
//{
//	int i;
//	unsigned int n = 0, t;
//	struct aiMatrix4x4 m = nd->mTransformation;
//
//	// update transform
//	aiTransposeMatrix4(&m);
//	glPushMatrix();
//	glMultMatrixf((float*)&m);
//
//	// draw all meshes assigned to this node
//	for (; n < nd->mNumMeshes; ++n) {
//		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
//
//		apply_material(sc->mMaterials[mesh->mMaterialIndex]);
//
//		if(mesh->mNormals == NULL) {
//			glDisable(GL_LIGHTING);
//		} else {
//			glEnable(GL_LIGHTING);
//		}
//
//		if(mesh->mColors[0] != NULL) {
//			glEnable(GL_COLOR_MATERIAL);
//		} else {
//			glDisable(GL_COLOR_MATERIAL);
//		}
//
//		for (t = 0; t < mesh->mNumFaces; ++t) {
//			const struct aiFace* face = &mesh->mFaces[t];
//			GLenum face_mode;
//
//			switch(face->mNumIndices) {
//				case 1: face_mode = GL_POINTS; break;
//				case 2: face_mode = GL_LINES; break;
//				case 3: face_mode = GL_TRIANGLES; break;
//				default: face_mode = GL_POLYGON; break;
//			}
//
//			glBegin(face_mode);
//
//			for(i = 0; i < face->mNumIndices; i++) {
//				int index = face->mIndices[i];
//				if(mesh->mColors[0] != NULL)
//					Color4f(&mesh->mColors[0][index]);
//				if(mesh->mNormals != NULL) 
//					glNormal3fv(&mesh->mNormals[index].x);
//				glVertex3fv(&mesh->mVertices[index].x);
//			}
//
//			glEnd();
//		}
//
//	}
//
//	// draw all children
//	for (n = 0; n < nd->mNumChildren; ++n) {
//		recursive_render(sc, nd->mChildren[n]);
//	}
//
//	glPopMatrix();
//}
// ----------------------------------------------------------------------------
void mesh::apply_material(const struct aiMaterial *mtl)
{
	float c[4];

	GLenum fill_mode;
	int ret1, ret2;
	struct aiColor4D diffuse;
	struct aiColor4D specular;
	struct aiColor4D ambient;
	struct aiColor4D emission;
	float shininess, strength;
	int two_sided;
	int wireframe;
	unsigned int max;	// changed: to unsigned

	int texIndex = 0;
	aiString texPath;	//contains filename of texture

	if(AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath))
	{
		//bind texture
		unsigned int texId = *textureIdMap[texPath.data];
		glBindTexture(GL_TEXTURE_2D, texId);
	}

	set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
		color4_to_float4(&diffuse, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
		color4_to_float4(&specular, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

	set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
		color4_to_float4(&ambient, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
		color4_to_float4(&emission, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

	max = 1;
	ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
	max = 1;
	ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
	if((ret1 == AI_SUCCESS) && (ret2 == AI_SUCCESS))
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
	else {
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
		set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
	}

	max = 1;
	if(AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
		fill_mode = wireframe ? GL_LINE : GL_FILL;
	else
		fill_mode = GL_FILL;
	glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

	max = 1;
	if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
}

//void mesh::apply_material(const struct aiMaterial *mtl)
//{
//	float c[4];
//
//	GLenum fill_mode;
//	int ret1, ret2;
//	struct aiColor4D diffuse;
//	struct aiColor4D specular;
//	struct aiColor4D ambient;
//	struct aiColor4D emission;
//	float shininess, strength;
//	int two_sided;
//	int wireframe;
//	unsigned int max;
//
//	set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
//	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
//		color4_to_float4(&diffuse, c);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);
//
//	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
//	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
//		color4_to_float4(&specular, c);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
//
//	set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
//	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
//		color4_to_float4(&ambient, c);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);
//
//	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
//	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
//		color4_to_float4(&emission, c);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);
//
//	max = 1;
//	ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
//	max = 1;
//	ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
//	if((ret1 == AI_SUCCESS) && (ret2 == AI_SUCCESS))
//		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
//	else {
//		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
//		set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
//		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
//	}
//
//	max = 1;
//	if(AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
//		fill_mode = wireframe ? GL_LINE : GL_FILL;
//	else
//		fill_mode = GL_FILL;
//	glPolygonMode(GL_FRONT_AND_BACK, fill_mode);
//
//	max = 1;
//	if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
//		glEnable(GL_CULL_FACE);
//	else 
//		glDisable(GL_CULL_FACE);
//}
//-----------------------------------------------------------------------------
bool mesh::loadasset(const std::string& path)
{
	//check if file exists
	std::ifstream fin(path.c_str());
	if(!fin.fail())
	{
		fin.close();
	}
	else
	{
		MessageBox(NULL, ("Couldn't open file: " + path).c_str() , "ERROR", MB_OK | MB_ICONEXCLAMATION);
		//logInfo( importer.GetErrorString());
		printf("Mesh '%s' not found\n", path.c_str());
		return false;
	}

	scene = importer.ReadFile( path, aiProcessPreset_TargetRealtime_Quality);

	// If the import failed, report it
	if( !scene)
	{
		//logInfo( importer.GetErrorString());
		printf("Mesh '%s' not found\n", path.c_str());
		return false;
	}

	// Now we can access the file's contents.
	//logInfo("Import of scene " + pFile + " succeeded.");

	printf("Loaded Mesh: '%s'\n", path.c_str());
	// We're done. Everything will be cleaned up by the importer destructor
	return true;
}
//bool mesh::loadasset (const char* path)
//{
//	// we are taking one of the postprocessing presets to avoid
//	// writing 20 single postprocessing flags here.
//	scene = aiImportFile(path,aiProcessPreset_TargetRealtime_Quality);
//
//	//Assimp::Importer Importer;
//
//    //scene = Importer.ReadFile(path.c_str(), aiProcessPreset_TargetRealtime_Quality);
//
//	if (scene) {
//		//get_bounding_box(&scene_min,&scene_max);
//		//scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
//		//scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
//		//scene_center.z = (scene_min.z + scene_max.z) / 2.0f;
//		//printf("Loaded Mesh: '%s'\n", path.c_str());
//		return true;
//	}
//	//printf("Error parsing '%s'\n", path.c_str());
//	return false;
//}
//-----------------------------------------------------------------------------



//void model::get_bounding_box_for_node (const struct aiNode* nd, 
//	struct aiVector3D* min, 
//	struct aiVector3D* max)
//	
//{
//	struct aiMatrix4x4 prev;
//	unsigned int n = 0, t;
//
//	for (; n < nd->mNumMeshes; ++n) {
//		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
//		for (t = 0; t < mesh->mNumVertices; ++t) {
//
//			struct aiVector3D tmp = mesh->mVertices[t];
//
//			min->x = aisgl_min(min->x,tmp.x);
//			min->y = aisgl_min(min->y,tmp.y);
//			min->z = aisgl_min(min->z,tmp.z);
//
//			max->x = aisgl_max(max->x,tmp.x);
//			max->y = aisgl_max(max->y,tmp.y);
//			max->z = aisgl_max(max->z,tmp.z);
//		}
//	}
//
//	for (n = 0; n < nd->mNumChildren; ++n) {
//		get_bounding_box_for_node(nd->mChildren[n],min,max);
//	}
//}
//
//
//void model::get_bounding_box (struct aiVector3D* min, struct aiVector3D* max)
//{
//
//	min->x = min->y = min->z =  1e10f;
//	max->x = max->y = max->z = -1e10f;
//	get_bounding_box_for_node(scene->mRootNode,min,max);
//}

bool mesh::loadtexture()
{
    ILboolean success;
 
    /* initialization of DevIL */
    ilInit();
 
    /* scan scene's materials for textures */
    for (unsigned int m=0; m<scene->mNumMaterials; m++)
    {
        int texIndex = 0;
        aiString path;  // filename
 
        aiReturn texFound = scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
        while (texFound == AI_SUCCESS) {
            //fill map with textures, OpenGL image ids set to 0
            textureIdMap[path.data] = 0;
            // more textures?
            texIndex++;
            texFound = scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
        }
    }
 
    int numTextures = textureIdMap.size();
 
    /* create and fill array with DevIL texture ids */
    ILuint* imageIds = new ILuint[numTextures];
    ilGenImages(numTextures, imageIds);
 
    /* create and fill array with GL texture ids */
    GLuint* textureIds = new GLuint[numTextures];
    glGenTextures(numTextures, textureIds); /* Texture name generation */
 
    /* get iterator */
    //std::map::iterator itr = textureIdMap.begin();
	std::map<std::string, GLuint*>::iterator itr = textureIdMap.begin();
    int i=0;
    for (; itr != textureIdMap.end(); ++i, ++itr)
    {
        //save IL image ID
		////		//save IL image ID
		////		std::string filename = (*itr).first;  // get filename
		////		(*itr).second =  &textureIds[i];	  // save texture id for filename in map
		////		itr++;								  // next texture

        std::string filename = (*itr).first;  // get filename
        (*itr).second = &textureIds[i];    // save texture id for filename in map
 
        ilBindImage(imageIds[i]); /* Binding of DevIL image name */
        ilEnable(IL_ORIGIN_SET);
        ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
        success = ilLoadImage((ILstring)filename.c_str());
 
        if (success) {
            /* Convert image to RGBA */
            ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
 
            /* Create and load textures to OpenGL */
            glBindTexture(GL_TEXTURE_2D, textureIds[i]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH),
                ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                ilGetData());
        }
        else
            printf("Couldn't load Image: %s\n", filename.c_str());
    }
    /* Because we have already copied image data into texture data
    we can release memory used by image. */
    ilDeleteImages(numTextures, imageIds);
 
    //Cleanup
    delete [] imageIds;
    delete [] textureIds;
 
    //return success;
    return true;
}

////bool mesh::loadtexture(const std::string& filename)
////{
////	ILboolean success;
////
////	/* Before calling ilInit() version should be checked. */
////	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
////	{
////		ILint test = ilGetInteger(IL_VERSION_NUM);
////		/// wrong DevIL version ///
////		std::string err_msg = "Wrong DevIL version. Old devil.dll in system32/SysWow64?";
////		char* cErr_msg = (char *) err_msg.c_str();
////		abortGLInit(cErr_msg);
////		printf("Wrong DevIL version");
////		return false;
////	}
////
////	ilInit(); /* Initialization of DevIL */
////
////	if (scene->HasTextures()) abortGLInit("Support for meshes with embedded textures is not implemented");
////
////	/* getTexture Filenames and Numb of Textures */
////	for (unsigned int m=0; m<scene->mNumMaterials; m++)
////	{
////		int texIndex = 0;
////		aiReturn texFound = AI_SUCCESS;
////
////		aiString path;	// filename
////
////		while (texFound == AI_SUCCESS)
////		{
////			texFound = scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
////			textureIdMap[path.data] = NULL; //fill map with textures, pointers still NULL yet
////			texIndex++;
////		}
////	}
////
////	int numTextures = textureIdMap.size();
////
////	/* array with DevIL image IDs */
////	ILuint* imageIds = NULL;
////	imageIds = new ILuint[numTextures];
////
////	/* generate DevIL Image IDs */
////	ilGenImages(numTextures, imageIds); /* Generation of numTextures image names */
////
////	/* create and fill array with GL texture ids */
////	textureIds = new GLuint[numTextures];
////	glGenTextures(numTextures, textureIds); /* Texture name generation */
////
////	/* define texture path */
////	std::string texturepath = "F:\\Alex\\Documents\\Visual Studio 2010\\Projects\\Hydro\\Hydro\\";
////
////	/* get iterator */
////	std::map<std::string, GLuint*>::iterator itr = textureIdMap.begin();
////
////	for (int i=0; i<numTextures; i++)
////	{
////
////		//save IL image ID
////		std::string filename = (*itr).first;  // get filename
////		(*itr).second =  &textureIds[i];	  // save texture id for filename in map
////		itr++;								  // next texture
////
////
////		ilBindImage(imageIds[i]); /* Binding of DevIL image name */
////		std::string fileloc = texturepath + filename;//filename;	/* Loading of image basepath + */
////		success = ilLoadImage(fileloc.c_str());
////
////		if (success) /* If no error occured: */
////		{
////			success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE); /* Convert every colour component into
////			unsigned byte. If your image contains alpha channel you can replace IL_RGB with IL_RGBA */
////			if (!success)
////			{
////				/* Error occured */
////				abortGLInit("Couldn't convert image");
////				printf("texture '%s' not converted\n", fileloc.c_str());
////				return false;
////			}
////			//glGenTextures(numTextures, &textureIds[i]); /* Texture name generation */
////			glBindTexture(GL_TEXTURE_2D, textureIds[i]); /* Binding of texture name */
////			//redefine standard texture values
////			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* We will use linear
////			interpolation for magnification filter */
////			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /* We will use linear
////			interpolation for minifying filter */
////			glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
////				ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
////				ilGetData()); /* Texture specification */
////		}
////		else
////		{
////			/* Error occured */
////			printf("texture '%s' not loaded\n", fileloc.c_str());
////			MessageBox(NULL, ("Couldn't load Image: " + fileloc).c_str() , "ERROR", MB_OK | MB_ICONEXCLAMATION);
////		}
////
////
////	}
////
////	ilDeleteImages(numTextures, imageIds); /* Because we have already copied image data into texture data
////	we can release memory used by image. */
////
////	//Cleanup
////	delete [] imageIds;
////	imageIds = NULL;
////
////	//return success;
////	return true;
////}
