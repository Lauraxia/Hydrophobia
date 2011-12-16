#include "includes/common.h"
#include "includes/FishList.h"
#include "includes/Fish.h"

using namespace std;




FishList::FishList(void)
{
	spawnRate = 300; //ms before a new fish appears
	timeElapsedSinceSpawn = 0; //when == spawnRate, new fish
}


FishList::~FishList(void)
{
}


bool newFishIsTooClose(int newLane)
{
	return false; //TODO: check to see if lane occupied, and preferably how far away neighbours are
}

//TODO needs to be rewritten to work with objects

bool isFirstFish = false;
int firstFish = 0;


void FishList::spawnNewFishIfNeeded()
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
		Fish newFish(1, 1, 5, 200);
		fishList.insert(std::make_pair(newFishLane, newFish));
		timeElapsedSinceSpawn = 0;

		//for debugging path of a fish in console
		if (!isFirstFish)
		{
			firstFish = newFishLane;
			isFirstFish = true;
		}
	}
}


void FishList::DeleteFish(int angle)
{
	if (fishList.count(angle)) //check that fish to be removed exists
	{
		fishList.erase(angle);
	}
}

void FishList::handleFishBite(int angle)
{
	//could have particle effect, or screen turning red, or something... tbd
	//for now, just delete offending fish
	DeleteFish(angle);
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
				handleFishBite(i);
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
		glRotatef(angle, 0,1,0);
		double x = fish.getPositionX();
		double y =  fish.getPositionY();
		glTranslatef(fish.getPositionX(), fish.getPositionY(), 0);

		glColor3f(1,0,1);
		glutSolidCube(fish.getSize()*0.5); //TODO: is this actually a good metric for deciding size, or scale?

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
	if (isFirstFish)
		printf("firstfish: '%d'\n", fishList[firstFish].getPositionX());
}

//called by main function, when testing for collisions with a potential fish target
Fish * FishList::GetFish(int angle)
{
	if (fishList.count(angle)) return &fishList[angle];
	return NULL;
}