#include "includes\common.h"
#include "includes\camera.h"
#include "includes\physics.h"
#include "includes\control.h"
#include "includes\mesh.h"

#include <stdio.h>

//Camera variables
float camInitPos[3] = {0,100,100};
Camera *cam = new Camera(camInitPos);
float rotAng[] = {0,0,0};

mesh *m1;
GLuint scene_list = 0;

float lightPosition[] = {10,10,10};

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

// Init GLEW
glewInit();
if (glewIsSupported("GL_VERSION_3_3"))
{
printf("Ready for OpenGL 3.3\n");
}
else
{
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
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluPerspective(100,0.6,0.01,1000);
glMatrixMode(GL_MODELVIEW);
glShadeModel(GL_SMOOTH); // Enable Smooth Shading
glClearColor(0.0f, 0.0f, 0.0f, 0.5f); // Black Background
glClearDepth(1.0f); // Depth Buffer Setup

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

void display()
{
//clear the screen
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glMatrixMode(GL_MODELVIEW);

glPushMatrix();


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

//This update the camera position
cam->setView();
glPushMatrix();
glCallList(scene_list);
glPopMatrix();

//Light
glLightfv(GL_LIGHT0,GL_POSITION,lightPosition);
//--------------------------------------------------------

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

glutIgnoreKeyRepeat(TRUE);

//register glut callbacks for keyboard and display function
glutKeyboardFunc(kbd);
glutPassiveMotionFunc(passiveMouse);
glutDisplayFunc(display);
glutKeyboardUpFunc(keyUp);

glutIdleFunc(idle);

m1 = new mesh("assets\\Inflatable boat.3ds");

//start the program!
glutMainLoop();

return 0;
}