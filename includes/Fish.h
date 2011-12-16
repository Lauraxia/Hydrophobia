#pragma once



class Fish
{
private:
	//TODO: figure out appropriate scale!
	static const int START_DISTANCE = 300; //how far away fish are from the origin when they begin their attack
	static const int SUCCESSFUL_ATTACK_THRESHOLD = 10; //when the fish gets this close, we get eaten
	static const int ANIMATION_FRAME_NUM = 60; //number of total animation frames all fish have

	static const int NUM_POSSIBLE_PATHS = 10; //every fish has one of these predefined paths
	static const int PATH_MIN_AMPLITUDE = 5; //min, max scaling of path vertically
	static const int PATH_MAX_AMPLITUDE = 20;
	static const int JUMP_BREADTH = 50; //how many frames long the jump path is (not necessarily finished by all fish)

	//specific fish information:
	int fishType; //the style of fish -- different models
	int fishSize;

	int pathType; //the trajectory type of the fish
	int jumpPoint; //frame where the jump begins

	int xPosition; //current x position of fish -- decreases till it reaches 0
	int yPosition;
	int rawXPositionRemainder; //milliseconds left over after last advance
	bool isJumping;

		//common fish information:
	static int **fishPaths; //each possible path = array from jump start point to 0


	int animationFrame; //for when jumping, where it is in the animation



public:
	Fish::Fish(int pathTypeValue, int typeOfFish, int sizeOfFish, int jumpPointX);
	Fish::Fish(void);
	~Fish(void);
	bool Fish::AdvancePosition(int milliseconds);
	int Fish::getPositionX();
	int Fish::getPositionY();
	int Fish::getSize();
private:
	static void setupFishPaths();
};

