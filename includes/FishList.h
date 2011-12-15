#pragma once
#include "includes/Fish.h"

class FishList
{
public:
	FishList(void);
	~FishList(void);
	int FishList::UpdateFishPositionBites(int milliseconds);
	void FishList::DrawAllFish();
	Fish * FishList::GetFish(int angle);
};

