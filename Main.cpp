#include "includes\common.h"
//#include "render.h"
//#include "scene.h"
//#include "scene_constructor.h"
#include "includes\camera.h"
#include "includes\physics.h"
#include "includes\control.h"

// Debug Stuff
//#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include "includes\mesh.h"

//#include <crtdbg.h>

//extern Node *world;
float camInitPos[3] = {0,100,0};
float camInitv[3] = {1,0,-1};
float camInitAng = -45;
Camera *cam = new Camera(camInitPos);
//float lastTime = 0;
float rotAng[] = {0,0,0};
float angle[] = {0,0,0};

mesh *m1;
GLuint scene_list = 0;
// ------------------------------------------------------------
//
// Model loading and OpenGL setup
//
void initialize()
{
	//Lighting Settings
	float amb[] = {0.33f, 0.22f, 0.03f, 0.5f}; 
	float diff[] = {0.78f, 0.57f, 0.11f, 0.50f}; 
	float spec[] = {0.15f, 0.4f, 0.8f, 0.5f}; 
	float shiny = 24;

	//glut initialization stuff:
	// set the window size, display mode, and create the window
	glutInitWindowSize(800, 800);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Hydrophobia");

	//	Init GLEW
	//glewExperimental = GL_TRUE;
	glewInit();
	if (glewIsSupported("GL_VERSION_3_3"))
		printf("Ready for OpenGL 3.3\n");
	else {
		printf("OpenGL 3.3 not supported\n");
		exit(1);
	}

	//enable Z buffer test, otherwise things appear in the order they're drawn
	glEnable(GL_DEPTH_TEST);

	//Backface culling
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	//setup the initial view
	// change to projection matrix mode, set the extents of our viewing volume
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	//gluPerspective(90, 1, 0.5, 2000);
	gluPerspective(100,0.6,0.01,1000);
	//set clear colour to white
	glClearColor(1, 1, 1, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glShadeModel(GL_SMOOTH); // Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f); // Black Background
	glClearDepth(1.0f); // Depth Buffer Setup

	glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Pretty Perspective Calculations :D

	//rotate camera initialy
	//glPushMatrix();
	//cam->rotate(camInitAng, camInitv);
	//glPopMatrix();

	//Lighting stuff
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);  
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff); 
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec); 
	
}

float position[] = {10,10,10};
void display()
{
	//clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	//cam->setView();

	glPushMatrix();
	
	

	//recursive_render(m1->scene,m1->scene->mRootNode);
	//------------------------------------------------------
	float tmp;


	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//gluLookAt(0.f,0.f,3.f,0.f,0.f,-5.f,0.f,1.f,0.f);

	// rotate it around the y axis
	//glRotatef(angle,0.f,1.f,0.f);

	// scale the whole asset to fit into our view frustum 
	//tmp = scene_max.x-scene_min.x;
	//tmp = aisgl_max(scene_max.y - scene_min.y,tmp);
	//tmp = aisgl_max(scene_max.z - scene_min.z,tmp);
	//tmp = 1.f / tmp;
	//glScalef(tmp, tmp, tmp);

        // center the model
	//glTranslatef( -scene_center.x, -scene_center.y, -scene_center.z );

        // if the display list has not been made yet, create a new one and
        // fill it with scene contents
	if(scene_list == 0) {
	    scene_list = glGenLists(1);
	    glNewList(scene_list, GL_COMPILE);
            // now begin at the root node of the imported data and traverse
            // the scenegraph by multiplying subsequent local transforms
            // together on GL's matrix stack.
		m1->render();
	    glEndList();
	}
	//glRotatef(rotAng[0],1,0,0);
	//glRotatef(rotAng[1],0,1,0);
	//glRotatef(rotAng[2],0,0,1);
	//glTranslated(0,-10,-20);
	cam->setView();
	glPushMatrix();
	glCallList(scene_list);
	glPopMatrix();
	glLightfv(GL_LIGHT0,GL_POSITION,position);
	//--------------------------------------------------------
//	physics(world);
//	render(world);
//	updateTime();

	glPopMatrix();
	//swap buffers - rendering is done to the back buffer, bring it forward to display
	glutSwapBuffers();
	updateTime();
	//force a redisplay, to keep the animation running
	glutPostRedisplay();
}

void idle()
{
	handleKeys();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	initialize();

	//set number of Display list elements that are needed
	//GLuint glGenLists(5);

	glutIgnoreKeyRepeat(TRUE);
	//register glut callbacks for keyboard and display function

	glutKeyboardFunc(kbd);
	glutSpecialFunc(special);
	glutPassiveMotionFunc(passiveMouse);
	glutDisplayFunc(display);
//	glutKeyboardUpFunc(keyUp);
	glutSpecialUpFunc(keySpecialUp);

	glutIdleFunc(idle);

	float pos[] = {0,0,0};
	
//	initPhysics();
//	constructScene();
	
	m1 = new mesh("assets\\Inflatable boat.3ds");
	//m1->loadasset(path);
	//look down from 45 degree angle
//	srand(deltaTime());
	
	//glRotatef(45,1,0,0);

	//start the program!
	glutMainLoop();

	return 0;
}