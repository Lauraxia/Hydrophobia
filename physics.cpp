#include <list>

#include "includes\physics.h"
#include "includes\common.h"
#include "includes\util.h"
//#include "models.h"

using namespace std;

//Environment Variables

float windVel[] = {1,0,0};
float *g;					//direction of gravity
float grav = 9.81;			//acceleration due to gravity
bool frictionMode = false;	//friction on the ground
extern bool wind;
extern bool explod;
extern bool rain;
//extern Node *world;

float lastTime = 0;
float deltaTime()
{   
	float dt = (float)(glutGet(GLUT_ELAPSED_TIME) - lastTime);
	return dt;
}
void updateTime()
{
	lastTime = (float)glutGet(GLUT_ELAPSED_TIME);
}
//void setGravity(float acceleration)
//{
//	grav = acceleration;
//}
//void setGravity(float direction[3],float acceleration)
//{
//	g = normalize(direction);
//	grav = acceleration;
//}
//void initPhysics()
//{
//	float dir[] = {0,-1,0};
//	setGravity(dir,9.81);
//}
//
//void explode(Node* node,float *position)
//{
//	float m_amb12[] = {1, 0, 0, 1.0}; 
//	float m_diff12[] = {1, 0, 0, 1.0}; 
//	float m_spec12[] = {1, 0, 0, 1}; 
//	float shiny12 = 54;
//
//	////// Create particle stream
//	float pos[] = {position[0],position[1]+1,position[2]};
//	//particleStream *explosion = new particleStream(pos,RENDER|AGES,EXPLOSION);
//
//	////stream->enable(RENDER|GRAVITY);
//	//float direction[] = {0,1,0};
//	//explosion->setStreamDirection(direction);
//	//explosion->model.loadModel("Cube",drawCube);
//	//explosion->setMaterial(m_amb12,m_diff12,m_spec12,shiny12);
//
//	//explosion->setScale(0.3);
//	//explosion->setVelocity(0.1);
//	//explosion->setDeviation(2);
//	//explosion->setMaxAge(1*1000);
//	//explosion->setRate(10);
//
//	//node->subNodes.push_front(explosion);
//
//
//
//	float pos1[] = {position[0],0,position[2]};
//	particleStream *stream2 = new particleStream(pos1,RENDER|AGES,FOUNTAIN);
//	//stream->enable(RENDER|GRAVITY);
//	float direction1[] = {0,1,0};
//	stream2->setStreamDirection(direction1);
//	stream2->model.loadModel("Sphere",drawCube);
//	stream2->setMaterial(m_amb12,m_diff12,m_spec12,shiny12);
//
//	stream2->setScale(0.3);
//	stream2->setVelocity(20);
//	stream2->setDeviation(0.4);
//	stream2->setMaxAge(0.5*1000);
//	stream2->setRate(30);
//
//	stream2->ages = true;
//
//	world->subNodes.push_front(stream2);
//}
//
//void physics(Node *node)
//{
//	if(node->physics)
//	{
//		//simple collision detection with zx plane
//		if(!explod && (node->position[1]) < 0 && fabs(node->position[0])<10 && fabs(node->position[2]) < 10)
//		{
//				//node->model.velocity[2] = -(node->model.velocity[2]);
//				float temp[3];
//				if(frictionMode)
//				{
//					if(!rain)
//					{
//						temp[0] =  node->model.velocity[0];
//						temp[1] = -node->model.velocity[1]*0.9;
//						temp[2] =  node->model.velocity[2];
//					}
//					else
//					{
//						temp[0] =  node->model.velocity[0];
//						temp[1] = -node->model.velocity[1]*0.1;
//						temp[2] =  node->model.velocity[2];
//					}
//				}
//				else
//				{
//					temp[0] =  node->model.velocity[0];
//					temp[1] = -node->model.velocity[1];
//					temp[2] =  node->model.velocity[2];
//				}
//				node->setVelocity(temp);
//		}
//		float position[3];
//		//float *dPos = multiply(node->model.velocity,deltaTime()/1000);
//		float d[3];
//		float m = deltaTime()/1000;
//		d[0] = m*node->model.velocity[0];
//		d[1] = m*node->model.velocity[1];
//		d[2] = m*node->model.velocity[2];
//
//		position[0] = node->position[0] + d[0];
//		position[1] = node->position[1] + d[1];
//		position[2] = node->position[2] + d[2];
//		node->setPosition(position);
//
//		float angle[3];
//		node->model.angle[0] = node->model.rotVel[0] + node->model.angle[0];
//		node->model.angle[1] = node->model.rotVel[1] + node->model.angle[1];
//		node->model.angle[2] = node->model.rotVel[2] + node->model.angle[2];
//
//		//node->setAngle(angle);
//	}
//
//	if(wind)
//	{
//		node->model.velocity[0] += 2*windVel[0]*deltaTime()/1000;
//		node->model.velocity[1] += 2*windVel[1]*deltaTime()/1000;
//		node->model.velocity[2] += 2*windVel[2]*deltaTime()/1000;
//	}
//	if(node->gravity)
//	{
//		//float *d = multiply(g,grav*deltaTime()/1000);
//
//		float d[3];
//		float m = grav*deltaTime()/1000;
//		d[0] = m*g[0];
//		d[1] = m*g[1];
//		d[2] = m*g[2];
//
//		float position[3];
//
//		node->model.velocity[0] = node->model.velocity[0] + d[0];
//		node->model.velocity[1] = node->model.velocity[1] + d[1];
//		node->model.velocity[2] = node->model.velocity[2] + d[2];
//	}
//	if(!node->subNodes.empty())
//	{
//		list<Node*>::iterator it;
//		for(it = node->subNodes.begin();it != node->subNodes.end();it++)
//		{
//
//			//Get rid of nodes if they have aged
//			if((*it)->ages)
//			{
//				if((*it)->age <= 0)
//				{
//					if(!(*it)->subNodes.empty())
//						(*it)->removeSubNodes();
//					delete *it;
//					it = node->subNodes.erase(it);
//					if(it==node->subNodes.end())
//						break;
//					continue;
//				}
//					(*it)->age -= deltaTime();
//			}
//
//			//explode
//			if(explod && ((*it)->position[1]) < 0 && fabs((*it)->position[0])<10 && fabs((*it)->position[2]) < 10 )
//			{
//				if(!((*it)->Type == EXPLOSION))
//				{
//					float p[3];
//
//					if(!(*it)->subNodes.empty())
//						(*it)->removeSubNodes();
//					p[0] = (*it)->position[0];
//					p[1] = (*it)->position[1];
//					p[2] = (*it)->position[2];
//
//					delete *it;
//					it = node->subNodes.erase(it);
//					explode(world,p);
//					if(it==node->subNodes.end())
//						break;
//					continue;
//				}
//			}
//
//			//Get rid of node if under y = 500
//			if((*it)->position[1]<-500)
//			{
//				/*if(!(*it)->subNodes.empty())
//					(*it)->removeSubNodes();*/
//				delete (*it);
//				it = node->subNodes.erase(it);
//				if(it==node->subNodes.end())
//					break;
//				continue;
//			}
//			physics(*it);
//		}
//	}
//}