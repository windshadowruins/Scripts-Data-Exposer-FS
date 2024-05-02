#pragma once
#include <vector>

#include "TargetNpcPosition.h"

static const int DEFAULT_COORDINATES = 42;
static const int SIZE_OF_CALL_INSTRUCTION = 5;
static const int ADDRESS_SIZE_IN_BYTES = 6;
static const int MAX_NUM_TARGETS = 5;
struct Position
{
	float x, y, z;
};

struct TargetNpcInfo
{
	long long targetBaseHandle;
	float x, y, z;
	float playerX, playerY, playerZ;
	long long targets[MAX_NUM_TARGETS] = {-1, -1, -1, -1, -1};
	int recordTargetsIndex = 0;
	int replayTargetIndex = 0;

	int getCoordinates(TargetNpcPosition positionIndex);

	float** updatePlayerCoordinates(intptr_t chrIns);

	void teleportTargetToPlayer(float* playerX_ptr) const;
	void teleportPlayerToTarget(float* playerX_ptr, float* targetX_ptr2) const;
	void speedUpEnemy() const;
	void teleport(int teleportType, float** playerCoordinatePointers);
	void addTarget(long long value);
	int nextTarget();
};
