#pragma once
#include <vector>

#include "BulletInfo.h"

struct BulletLog
{
	size_t numBullets;
	std::vector<BulletInfo> bullets;
	intptr_t processBaseHandle;

	BulletLog(intptr_t processBaseHandle);
	void add(BulletInfo bullet);
	void teleport(int targetBulletID, const CoordinatePointers& playerCoordinatePointers) const;
	static void teleportPlayerTo(float* from, float* to);
};
