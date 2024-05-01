#pragma once
#include "TargetNpcPosition.h"

static const int DEFAULT_COORDINATES = 42;
static const int SIZE_OF_CALL_INSTRUCTION = 5;
static const int ADDRESS_SIZE_IN_BYTES = 6;

struct Position
{
	float x, y, z;
};

struct TargetNpcInfo
{
	long long baseHandle;
	float x, y, z;
	float playerX, playerY, playerZ;

	int getCoordinates(TargetNpcPosition positionIndex);

	float** updatePlayerCoordinates(intptr_t chrIns);

	void teleportTargetToPlayer(float* playerX_ptr) const;
	void teleportPlayerToTarget(float* playerX_ptr) const;
	void speedUpEnemy() const;
	void teleport(int teleportType, float** playerCoordinatePointers);
};
