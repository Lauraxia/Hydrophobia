#include "render.h"
#include "util.h"
#include <Assimp/assimp.hpp>	
#include <Assimp/aiPostProcess.h>
#include <Assimp/aiScene.h>
#include <map>
// ------------------------------------------------------------
//
// Render stuff
//

// Render Assimp Model

void recursive_render (const struct aiScene *sc, const struct aiNode* nd)
{

	// Get node transformation matrix
	struct aiMatrix4x4 m = nd->mTransformation;
	// OpenGL matrices are column major
	m.Transpose();

	// save model matrix and apply node transformation
	pushMatrix();

	float aux[16];
	memcpy(aux,&m,sizeof(float) * 16);
	multMatrix(modelMatrix, aux);
	setModelMatrix();


	// draw all meshes assigned to this node
	for (unsigned int n=0; n < nd->mNumMeshes; ++n){
		// bind material uniform
		glBindBufferRange(GL_UNIFORM_BUFFER, materialUniLoc, myMeshes[nd->mMeshes[n]].uniformBlockIndex, 0, sizeof(struct MyMaterial));	
		// bind texture
		glBindTexture(GL_TEXTURE_2D, myMeshes[nd->mMeshes[n]].texIndex);
		// bind VAO
		glBindVertexArray(myMeshes[nd->mMeshes[n]].vao);
		// draw
		glDrawElements(GL_TRIANGLES,myMeshes[nd->mMeshes[n]].numFaces*3,GL_UNSIGNED_INT,0);

	}

	// draw all children
	for (unsigned int n=0; n < nd->mNumChildren; ++n){
		recursive_render(sc, nd->mChildren[n]);
	}
	popMatrix();
}


// Rendering Callback Function

void renderScene(void) {

	static float step = 0.0f;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set camera matrix
	setCamera(camX,camY,camZ,0,0,0);

	// set the model matrix to the identity Matrix
	setIdentityMatrix(modelMatrix,4);

	// sets the model matrix to a scale matrix so that the model fits in the window
	scale(scaleFactor, scaleFactor, scaleFactor);

	// keep rotating the model
	rotate(step, 0.0f, 1.0f, 0.0f);
	
	// use our shader
	glUseProgram(program);

	// we are only going to use texture unit 0
	// unfortunately samplers can't reside in uniform blocks
	// so we have set this uniform separately
	glUniform1i(texUnit,0);

	recursive_render(scene, scene->mRootNode);

	// FPS computation and display
	frame++;
	time=glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		sprintf(s,"FPS:%4.2f",
			frame*1000.0/(time-timebase));
		timebase = time;
		frame = 0;
		glutSetWindowTitle(s);
	}

	// swap buffers
	glutSwapBuffers();

	// increase the rotation angle
	//step++;
}