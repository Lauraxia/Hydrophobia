#include "includes\drawWorld.h"

void drawWorld()
{

	
	scene_list = glGenLists(1);
	glNewList(scene_list, GL_COMPILE);
        // now begin at the root node of the imported data and traverse
        // the scenegraph by multiplying subsequent local transforms
        // together on GL's matrix stack.
	//boat->render();
	glEndList();
}