#pragma once
#include "includes/Fish.h"

#include <map>
class FishList
{
private:
	std::map<int, Fish> fishList; //any fish on screen, from 0 to 360 degrees
	int spawnRate; //ms before a new fish appears
	int timeElapsedSinceSpawn; //when == spawnRate, new fish
	void FishList::handleFishBite(int angle);
public:
	FishList(void);
	~FishList(void);
	int FishList::UpdateFishPositionBites(int milliseconds);
	void FishList::DrawAllFish();
	Fish * FishList::GetFish(int angle);
	void FishList::DeleteFish(int angle);
private:
	void spawnNewFishIfNeeded();
};

