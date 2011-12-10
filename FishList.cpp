#include "includes/common.h"
#include "includes/FishList.h"
#include "includes/Fish.h"
#include <map>
using namespace std;


std::map<int, Fish> fishList; //any fish on screen, from 0 to 360 degrees
int spawnRate = 3000; //ms before a new fish appears
int timeElapsedSinceSpawn = 0; //when == spawnRate, new fish

FishList::FishList(void)
{
	

}


FishList::~FishList(void)
{
}

//called from main loop -- advance fish, return how many of them have bitten us just now:
int FishList::UpdateFishPositionBites(int milliseconds)
{
	int fishBites = 0;
	//update positions of all fish in the list:
	for (int i=0; i<360; i++)
	{
		if (fishList.count(i)) //there's a fish at this angle
		{
			if (!fishList[i].AdvancePosition(milliseconds)) //the fish bit us!
			{
				fishBites++;
			}
		}
	}
	return fishBites;
}

void drawFish(Fish fish, int angle)
{
	//for now, just draw a cube...
	glPushMatrix();
		glTranslatef(fish.getPositionX(), fish.getPositionY(), 0);
		glRotatef(angle, 0,1,0);
		glutSolidCube(fish.getSize());

	glPopMatrix();
}

//called from main loop -- render fish
void FishList::DrawAllFish()
{
	for (int i=0; i<360; i++)
	{
		if (fishList.count(i)) //there's a fish at this angle
		{
			drawFish(fishList[i], i);
		}
	}
}

