#include "includes/Fish.h"
#include <math.h>
#include "includes\common.h"

//TODO: figure out appropriate scale!
const int START_DISTANCE = 5000; //how far away fish are from the origin when they begin their attack
const int SUCCESSFUL_ATTACK_THRESHOLD = 50; //when the fish gets this close, we get eaten
const int ANIMATION_FRAME_NUM = 60; //number of total animation frames all fish have

int fishType; //the style of fish -- different models
int fishSize;

int pathType; //the trajectory type of the fish
int jumpPoint; //frame where the jump begins

int xPosition = START_DISTANCE; //current x position of fish -- decreases till it reaches 0
int rawXPositionRemainder = 0; //milliseconds left over after last advance
bool isJumping = false;

int animationFrame = 0; //for when jumping, where it is in the animation

Fish::Fish(int pathTypeValue, int typeOfFish, int sizeOfFish, int jumpPointX)
{
	pathType = pathTypeValue;
	fishType = typeOfFish;
	jumpPoint = jumpPointX;
	fishSize = sizeOfFish;
}

Fish::Fish(void) //apparently maps need a default constructor, so here it is... never used though
{
}


Fish::~Fish(void)
{
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
			//advance animation
			animationFrame++;
			if (animationFrame > ANIMATION_FRAME_NUM)
			{
				animationFrame = 0;
			}
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
	//TODO: hook this up, either by storing ypos each advance, or by looking up from elsewhere
	return 0;
}

int Fish::getSize() //used by drawing function in FishList
{
	return fishSize;
}