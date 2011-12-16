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

extern bool frictionMode;
bool rain = false;
bool pauseParticles = false;
bool reset = false;
bool wind = false;
bool explod = false;
bool gainMomentum = false;

float invspeed = 20;
int shadeMode = 1;



bool light = true;
bool keys[256] = {false};
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
	///////////////////////////////////
	///KEBOARD KEYS
	/////////////////////////////

	//if the "q" key is pressed, quit the program
	if(keys['q'])
	{
		exit(0);
	}

	// f - toggles friction mode
	if(keys['f'] && !keyUsd['f'])
	{
		frictionMode = !frictionMode;
		keyUsd['f'] = true;
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
	// spacebar - starts/stops simulation
	if(keys[' '] && !keyUsd[' '])
	{
		pauseParticles = !pauseParticles;
		keyUsd[' '] = true;
	}
	// r - resets simulation
		if(keys['r'] && !keyUsd['r'] )
	{
		reset = !reset;
		keyUsd['r'] = true;
	}

	// w toggles wind
	if(keys['w'] && !keyUsd['w'] )
	{
		wind = !wind;
		keyUsd['w'] = true;
	}

		//Toggles Explosion mode
	if(keys['e'] && !keyUsd['e'] )
	{
		explod = !explod;
		keyUsd['e'] = true;
	}

	//Toggle rain
	if(keys['a'] && !keyUsd['a'] )
	{
		rain = !rain;
		keyUsd['a'] = true;
	}


	// wind control
	if(wind)
	{
		//Left
		if(keys['4'] && !keyUsd['4'])
		{
			windVel[0] = 1;
			windVel[1] = 0; 
			windVel[2] = 0;
			keyUsd['w'] = true;
		}
		//Right
		if(keys['6'] && !keyUsd['6'])
		{
			windVel[0] = -1;
			windVel[1] = 0; 
			windVel[2] = 0;
			keyUsd['w'] = true;
		}
		//Up
		if(keys['8'] && !keyUsd['8'])
		{
			windVel[0] = 0;
			windVel[1] = 0; 
			windVel[2] = 1;
			keyUsd['w'] = true;
		}
		//Down
		if(keys['5'] && !keyUsd['5'])
		{
			windVel[0] = 0;
			windVel[1] = 0; 
			windVel[2] = -1;
			keyUsd['w'] = true;
		}
	}


	////////////////////////////////
	//SPECIAL KEYS
	////////////////////////////////
	float direction[3] = {0,0,0};
	if(specials[GLUT_KEY_RIGHT])
	{
		rotAng[1] = rotAng[1] + deltaTime()/10;
		/*direction[0] = 1;
		cam->move(direction,deltaTime()/invspeed);*/
	}
	if(specials[GLUT_KEY_LEFT])
	{
		rotAng[1] = rotAng[1] -deltaTime()/10;

		/*direction[0] = -1;
		cam->move(direction,deltaTime()/invspeed);*/
	}
	if(specials[GLUT_KEY_UP])
	{
		rotAng[0] = rotAng[0] + deltaTime()/10;
		//rotAng[1] = deltaTime();
		/*direction[2] = 1;
		cam->move(direction,deltaTime()/invspeed);*/
	}
	if(specials[GLUT_KEY_DOWN])
	{
		rotAng[0] = rotAng[0] - deltaTime()/10;
		//rotAng[1] = -deltaTime();
		/*direction[2] = -1;
		cam->move(direction,deltaTime()/invspeed);*/
	}

}