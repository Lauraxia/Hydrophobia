#include "includes/Fish.h"
#include <math.h>
#include "includes\common.h"

static bool pathsInitialized = false; //set to true after setupFishPaths run first time from constructor

//Static 2D Array initialization
int** Fish::fishPaths = new int*[NUM_POSSIBLE_PATHS];

Fish::Fish(int pathTypeValue, int typeOfFish, int sizeOfFish, int jumpPointX)
{
	xPosition = START_DISTANCE; //current x position of fish -- decreases till it reaches 0
	yPosition = 0;
	rawXPositionRemainder = 0; //milliseconds left over after last advance
	isJumping = false;
	animationFrame = 0; //for when jumping, where it is in the animation


	pathType = pathTypeValue;
	fishType = typeOfFish;
	jumpPoint = jumpPointX;
	fishSize = sizeOfFish;
	if (!pathsInitialized)
	{

		setupFishPaths(); //done only once
	}
}

Fish::Fish(void) //apparently maps need a default constructor, so here it is... never used though
{
	xPosition = START_DISTANCE; //current x position of fish -- decreases till it reaches 0
	yPosition = 0;
	rawXPositionRemainder = 0; //milliseconds left over after last advance
	isJumping = false;

	animationFrame = 0; //for when jumping, where it is in the animation

	if (!pathsInitialized)
	{
		setupFishPaths(); //done only once
	}
}


Fish::~Fish(void)
{
	//for(int i = 0;i < NUM_POSSIBLE_PATHS; i++)
	//	delete fishPaths[i];
	//delete fishPaths;
}


bool Fish::AdvancePosition(int milliseconds) //returns false if successful attack
{
	//add newly elapsed ms to those left over from last time -- prevents frame dropping
	rawXPositionRemainder += milliseconds;

	//assuming 1000 ms/sec * 1/60 sec/frame = 16.6667 ms/frame -- and reduce position since nearing origin:
	int movement = floor(rawXPositionRemainder / 16.6667);
	xPosition -= movement;
	rawXPositionRemainder -= floor(movement*16.6667);

	if (xPosition <= SUCCESSFUL_ATTACK_THRESHOLD) //the fish has got us!
	{
		return false;
	}
	else //advance position as usual
	{
		//TODO: have y position here? or take care of in external class?
		if (isJumping)
		{
			yPosition = fishPaths[pathType][xPosition - jumpPoint];
			//advance animation
			animationFrame++;
			if (animationFrame > ANIMATION_FRAME_NUM)
			{
				animationFrame = 0;
			}
		}
		else
		{
			yPosition = 0; //TODO: really necessary every time? probably not...
		}
	}
	return true;
}

int Fish::getPositionX() //used by drawing function in FishList
{
	return xPosition;
}

int Fish::getPositionY() //used by drawing function in FishList
{
	//ypos updated each time in advance
	return yPosition;
}

int Fish::getSize() //used by drawing function in FishList
{
	return fishSize;
}

//must be called once at the start -- done so invisibly from normal constructor
void Fish::setupFishPaths()
{
	for(int i = 0;i < NUM_POSSIBLE_PATHS; i++)
		fishPaths[i] = new int[JUMP_BREADTH];
	//TODO: evaluate quality of these paths -- just a rough cut for now
	for (int i = 0; i<NUM_POSSIBLE_PATHS; i++)
	{
		for (int j=0; j<JUMP_BREADTH; j++)
		{
			double currAmplitude = (PATH_MIN_AMPLITUDE + (i/(double)NUM_POSSIBLE_PATHS * (PATH_MAX_AMPLITUDE - PATH_MIN_AMPLITUDE)));
			double currAngularFrequency = 1; //TODO: add proper formula here
			fishPaths[i][j] = -1 * currAmplitude * sin(currAngularFrequency*j);
		}
	}
	pathsInitialized = true;
}