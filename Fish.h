#pragma once
class Fish
{
public:
	Fish::Fish(int pathTypeValue, int typeOfFish, int sizeOfFish, int jumpPointX);
	Fish::Fish(void);
	~Fish(void);
	bool Fish::AdvancePosition(int milliseconds);
	int Fish::getPositionX();
	int Fish::getPositionY();
	int Fish::getSize();
};

