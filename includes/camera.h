#include "includes\common.h"
#ifndef CAMERA
#define CAMERA

class Camera
{
	float postion[3];
private:
	float matrix[16];
public:
	Camera(float pos[]);
	void setView();
	void move(float pos[], float distance);
	void setPosition(float pos[]);
	void rotate(float deg, float refNormal[]);
	void gRotate(float deg, float ref[]);
};

/*void camera(float* v);
void rotate(float x, float y);
void forward();
void back();
void right();
void left();
void updateCam();
*/
#endif