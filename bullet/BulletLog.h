#pragma once
#include <vector>

#include "BulletInfo.h"

struct BulletLog
{
	int numBullets;
	std::vector<BulletInfo> bullets;
	intptr_t processBaseHandle;

	BulletLog(intptr_t processBaseHandle);
	void add(BulletInfo bullet);
	void teleport(int targetBulletID, const CoordinatePointers& playerCoordinatePointers) const;
};
