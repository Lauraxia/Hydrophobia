#include "includes\projectile.h"
#include "includes\physics.h"

using namespace std;
//-------------------------------------------------------------------------
//Projectile 
//-------------------------------------------------------------------------
projectile::projectile(){}
projectile::~projectile(){}
void projectile::render()
{

}
void projectile::move()
{
	position[0] = position[0] + velocity[0]*deltaTime();
	position[1] = position[1] + velocity[1]*deltaTime();
	position[2] = position[2] + velocity[2]*deltaTime();
}


//--------------------------------------------------------------------------
//Projectile List
//--------------------------------------------------------------------------
projectileList::projectileList(){}
projectileList::~projectileList(){}

//renders all the projectiles
void projectileList::render()
{
	list<projectile>::iterator it;
	for ( it=projectiles.begin() ; it != projectiles.end(); it++ )
	  it->render();
}