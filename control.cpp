#include "includes\control.h"
//#include "util.h"
#include "includes\physics.h"
#include <math.h>
#include <stdio.h>

//#include "scene.h"
/* kbd -- the GLUT keyboard function 
 *  key -- the key pressed
 *  x and y - mouse x and y coordinates at the time the function is called
 */
extern Camera *cam;
extern float rotAng[];
extern float windVel[];

extern float angle[3];

bool reset = false;
bool fire = false;

float invspeed = 20;
int shadeMode = 1;



bool light = true;
bool keys[256] = {false};
bool mouseButton[256] = {false};
bool specials[256] = {false};
bool keyUsd[256] = {false};

void keyUp(unsigned char key, int x, int y)
{
	keys[key] = false;
	keyUsd[key] = false;
}
void keySpecialUp(int key, int x, int y)
{
	specials[key] = false;
}
void kbd(unsigned char key, int x, int y)
{
	keys[key] = true;
}

void special(int key, int x, int y)
{
	float direction[3] = {0,0,0};

	if(specials[key] == false)
		specials[key] = true;
}
void mouse(int button, int state, int x, int y)
{
	mouseButton[button] = !mouseButton[button];
}
void passiveMouse(int x,int y)
{
	static int centerX = glutGet(GLUT_WINDOW_WIDTH) / 2;
    static int centerY = glutGet(GLUT_WINDOW_HEIGHT) / 2;
	static float xAngle = 0;
	static float yAngle = 0;

	//change in width, and height
	float dw = (float)(x - centerX);
	float dh = (float)(y - centerY);

	if(dw != 0 || dh != 0)
	{

		float ref[3] = {1,0,0};
		//float *ref = normalize(ref1);
		//yAngle < -160 && dy > 0 || yAngle > 160 && dy < 0 ||
		//if( yAngle < 160 && yAngle > -160)
		//{
			xAngle = dh*deltaTime()/100;
			cam->rotate(dh*deltaTime()/100,ref);

		//}
		//printf("Angle: '%f'\n", yAngle);

		ref[0] = 0;
		ref[1] = 1;

		yAngle = dw*deltaTime()/100;
		cam->gRotate(dw*deltaTime()/100,ref);

		angle[0] += xAngle;
		angle[1] += yAngle;
			
		if(angle[0] >360)
			angle[0] = angle[0] - 360;
		else if(angle[0] < 0)
			angle[0] = 360 + angle[0];

		if(angle[1] >360)
			angle[1] = angle[1] - 360;
		else if(angle[1] < 0)
			angle[1] = 360 + angle[1];

		glutWarpPointer(centerX, centerY);
	}
}

void handleKeys()
{
	/////////////////////////////
	//MOUE KEYS
	///////////////////////////
	if(mouseButton[GLUT_LEFT_BUTTON])
	{
		fire = true;
	}

	///////////////////////////////////
	///KEBOARD KEYS
	/////////////////////////////

	//if the "q" key is pressed, quit the program
	if(keys['q'])
	{
		exit(0);
	}
	// l - toggles Lighting mode
	if(keys['l'] && !keyUsd['l'])
	{
		light = !light;
		if(light)
			glEnable(GL_LIGHTING);
		else
			glDisable(GL_LIGHTING);
		keyUsd['l'] = true;
	}
}