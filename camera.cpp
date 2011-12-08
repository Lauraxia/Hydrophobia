#include "camera.h"
//#include "util.h"

// Matrix Manipulated Camera
// With help from http://www.flipcode.com/archives/OpenGL_Camera.shtml
// by Alexander Festini

Camera::Camera()
{
	float pos[] = {0,0,0};
	memset(matrix,0,16*sizeof(float));
	matrix[0] = 1;
	matrix[5] = 1;
	matrix[10]  = -1;
	matrix[15] = 1;

	matrix[12] = pos[0];
	matrix[13] = pos[1];
	matrix[14] = pos[2];
}
Camera::Camera(float pos[])
{
	memset(matrix,0,16*sizeof(float));
	matrix[0] = 1;
	matrix[5] = 1;
	matrix[10]  = -1;
	matrix[15] = 1;

	matrix[12] = pos[0];
	matrix[13] = pos[1];
	matrix[14] = pos[2];
}

void Camera::setView() 
{
	//printf("x = %f y = %f z = %f\n",matrix[12],matrix[13],matrix[14]);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	float viewmatrix[16]={//Remove the three - for non-inverted z-axis
						  matrix[0], matrix[4], -matrix[8], 0,
						  matrix[1], matrix[5], -matrix[9], 0,
						  matrix[2], matrix[6], -matrix[10], 0,

						  -(matrix[0]*matrix[12] +
						  matrix[1]*matrix[13] +
						  matrix[2]*matrix[14]),

						  -(matrix[4]*matrix[12] +
						  matrix[5]*matrix[13] +
						  matrix[6]*matrix[14]),

						  //add a - like above for non-inverted z-axis
						  (matrix[8]*matrix[12] +
						  matrix[9]*matrix[13] +
						  matrix[10]*matrix[14]), 1};
	glLoadMatrixf(viewmatrix);
}
void Camera::move(float pos[], float distance) 
{
	float dx=pos[0]*matrix[0] + pos[1]*matrix[4] + pos[2]*matrix[8];
	float dy=pos[0]*matrix[1] + pos[1]*matrix[5] + pos[2]*matrix[9];
	float dz=pos[0]*matrix[2] + pos[1]*matrix[6] + pos[2]*matrix[10];
	matrix[12] += dx * distance;
	matrix[13] += dy * distance;
	matrix[14] += dz * distance;
}
void Camera::setPosition(float pos[]) 
{
	matrix[12] = pos[0];
	matrix[13] = pos[1];
	matrix[14] = pos[2];
}
void Camera::rotate(float deg, float ref[]) 
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadMatrixf(matrix);
	glRotatef(deg, ref[0],ref[1],ref[2]);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	glPopMatrix();
}

void Camera::gRotate(float deg, float ref[]) {
	float dx=ref[0]*matrix[0] + ref[1]*matrix[1] + ref[2]*matrix[2];
	float dy=ref[0]*matrix[4] + ref[1]*matrix[5] + ref[2]*matrix[6];
	float dz=ref[0]*matrix[8] + ref[1]*matrix[9] + ref[2]*matrix[10];
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadMatrixf(matrix);
	glRotatef(deg, dx,dy,dz);
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	glPopMatrix();
}
