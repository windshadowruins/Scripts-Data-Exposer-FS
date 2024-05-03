#pragma once
#include <vector>

#include "CoordinatePointers.h"
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
	intptr_t processBaseAddress;
	long long targetBaseHandle;
	float x, y, z;
	float playerX, playerY, playerZ;
	long long targets[MAX_NUM_TARGETS] = {-1, -1, -1, -1, -1};
	int recordTargetsIndex = 0;
	int replayTargetIndex = 0;

	TargetNpcInfo(intptr_t processBaseAddress);
	int getCoordinates(TargetNpcPosition positionIndex);

	CoordinatePointers updatePlayerCoordinates(intptr_t chrIns);

	void teleportTargetToPlayer(float* playerX_ptr) const;
	void teleportPlayerToTarget(float* playerX_ptr) const;
	void teleportPlayerToList(float* playerX_ptr);
	static void teleportPlayerTo(float* playerX_ptr, intptr_t destinationX_address);
	void speedUpEnemy() const;
	void resetTeleportList();
	static bool isLoaded(intptr_t characterBaseHandle, std::vector<long long>& allCharacters);
	static bool isValidCharacter(intptr_t characterBaseHandle, std::vector<intptr_t>& allCharacters);
	static bool isAlive(intptr_t characterBaseHandle);
	void teleport(int teleportType, const CoordinatePointers& playerCoordinatePointers);
	void addTarget(long long value);
	static int getHP(long long target);
	static std::vector<intptr_t> allLoadedCharacters(intptr_t processBase);
};
