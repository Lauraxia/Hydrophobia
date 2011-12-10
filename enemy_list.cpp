#include "includes\enemy_list.h"
#include "includes\physics.h"
//class Enemy
//{
//public:
//	float distance;
//	float height;
//public:
//	Enemy(mesh e);
//	~Enemy();
//	void updatePosition();
//	void render();
//private:
//	GLuint DLid;
//	float distanceThreshold;
//	float heightMax;
//};

Fish::Fish(GLuint list, float angle)
{
	DLid = list;
	attackAngle = angle;
	init();
}
Fish::Fish(mesh *e, float angle)
{
	attackAngle = angle;
	// make a list
	DLid = glGenLists(1);
	glNewList(DLid, GL_COMPILE);
	e->render();
	glEndList();
	init();
}
Fish::~Fish()
{
}
void Fish::init()
{
	srand(glutGet(GLUT_ELAPSED_TIME));
	
}
void Fish::updatePosition()
{
	position[0] = distance*sin(attackAngle);

	// Starts moving up at the threshold
	if(distance < distanceThreshold)
		position[1] = 1;
	else
		position[1] = 0;


	position[2] = distance*cos(attackAngle);
}
void Fish::move()
{
	distance -= deltaTime()*speed;
	updatePosition();
}
void Fish::render()
{
	glPushMatrix();
	glCallList(DLid);
	glPopMatrix();
}
void Fish::setSpeed(float speed)
{
	this->speed = speed;
}
void Fish::setHeight(float height)
{
	this->height = height;
}
void Fish::setDistance(float distance)
{
	this->distance = distance;
}