#include "includes/common.h"
#include "includes/FishList.h"
#include "includes/Fish.h"
#include <map>
using namespace std;


std::map<int, Fish> fishList; //any fish on screen, from 0 to 360 degrees
int spawnRate = 300; //ms before a new fish appears
int timeElapsedSinceSpawn = 0; //when == spawnRate, new fish

FishList::FishList(void)
{
	

}


FishList::~FishList(void)
{
}


bool newFishIsTooClose(int newLane)
{
	return false; //TODO: check to see if lane occupied, and preferably how far away neighbours are
}

void spawnNewFishIfNeeded()
{
	if (timeElapsedSinceSpawn >= spawnRate)
	{
		int nextLane = rand()%360;
		int newFishLane = nextLane;
		while (newFishIsTooClose(newFishLane))
		{
			newFishLane++;
			if (newFishLane == nextLane)
			{
				break; //already tried everywhere, and there is no good position. So pick this anyway.
			}
			else if (newFishLane >= 360)
			{
				newFishLane = 0; //wrap around
			}
		}
		Fish newFish(1, 1, 5, 2000);
		fishList.insert(std::make_pair(newFishLane, newFish));
		timeElapsedSinceSpawn = 0;
	}
}

//called from main loop -- advance fish, return how many of them have bitten us just now,
//as a sum of their sizes:
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
				fishBites+= fishList[i].getSize();
			}
		}
	}

	//new fish are automatically added to keep up with desired spawn rate:
	timeElapsedSinceSpawn += milliseconds;
	spawnNewFishIfNeeded();

	return fishBites;
}

void drawFish(Fish fish, int angle)
{
	//for now, just draw a cube...
	glPushMatrix();
		glTranslatef(fish.getPositionX(), fish.getPositionY(), 0);
		glRotatef(angle, 0,1,0);
		glColor3f(1,0,1);
		glutSolidCube(fish.getSize()*100); //TODO: is this actually a good metric for deciding size, or scale?

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

//called by main function, when testing for collisions with a potential fish target
Fish * FishList::GetFish(int angle)
{
	if (fishList.count(angle)) return &fishList[angle];
	return NULL;
}