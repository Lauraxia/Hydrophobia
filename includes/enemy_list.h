#include "includes\common.h"
#include "includes\mesh.h"

//--------------------------------------------------------------------
//Enemy Classes
//-------------------------------------------------------------------
class Fish
{
public:
	float attackAngle;
	float distance;
	float height;
	float speed;
public:
	Fish(GLuint list, float angle);
	Fish(mesh *e, float angle);
	~Fish();
	void render();
	void move();
	void setSpeed(float speed);
	void setHeight(float height);
	void setDistance(float distance);
private:
	GLuint DLid;
	float distanceThreshold;
	float heightMax;
	float position[3];
private:
	void updatePosition();
	void init();
};