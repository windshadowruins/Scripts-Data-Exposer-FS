#include "pch.h"
#include "target/TargetNpcInfo.h"

#include "Logger.h"
#include "mem/prot_flags.h"
#include "target/TargetNpcPosition.h"

int TargetNpcInfo::getCoordinates(TargetNpcPosition positionIndex)
{
	Logger::debug("TARGET BASE = %p, ", this);
	Logger::debug("TARGET BASE X = %p\n", &x);
	Logger::debug("TARGET LOCATION = (%f, %f, %f)\n", x, y, z);
	switch (positionIndex)
	{
	case TARGET_NPC_X: return *((intptr_t*)&x) & 0xFFFFFFFF;
	case TARGET_NPC_Y: return *((intptr_t*)&y) & 0xFFFFFFFF;
	case TARGET_NPC_Z: return *((intptr_t*)&z) & 0xFFFFFFFF;
	default: return mem::prot_flags::INVALID;
	}
}

float** TargetNpcInfo::updatePlayerCoordinates(intptr_t chrIns)
{
	float* playerX_ptr = (float*)(*(intptr_t*)(*(intptr_t*)(chrIns + 0x190) + 0x68) + 0x70);
	float* playerY_ptr = (float*)(*(intptr_t*)(*(intptr_t*)(chrIns + 0x190) + 0x68) + 0x74);
	float* playerZ_ptr = (float*)(*(intptr_t*)(*(intptr_t*)(chrIns + 0x190) + 0x68) + 0x78);

	playerX = *playerX_ptr;
	playerY = *playerY_ptr;
	playerZ = *playerZ_ptr;

	float* coordinatePtrs[3] = {playerX_ptr, playerY_ptr, playerZ_ptr};
	return coordinatePtrs;
}

void TargetNpcInfo::teleportTargetToPlayer(float* playerX_ptr) const
{
	float* targetX_ptr = (float*)(*(long long*)(*(long long*)(targetBaseHandle + 0x190) + 0x68) + 0x70);
	memcpy(targetX_ptr, playerX_ptr, 12);
}

void TargetNpcInfo::teleportPlayerToTarget(float* playerX_ptr, float* targetX_ptr2) const
{
	// float* targetX_ptr = (float*)(*(long long*)(*(long long*)(targetBaseHandle + 0x190) + 0x68) + 0x70);
	memcpy(playerX_ptr, targetX_ptr2, 12);
}

void TargetNpcInfo::speedUpEnemy() const
{
	float* targetSpeedModifier_ptr = (float*)(*(long long*)(*(long long*)(targetBaseHandle + 0x190) + 0x28) + 0x17C8);
	*targetSpeedModifier_ptr = 25.0;
}

void TargetNpcInfo::teleport(int teleportType, float** playerCoordinatePointers)
{

	float* playerX_ptr = playerCoordinatePointers[0];

	if (x == 42 && y == 42 && z == 42) return;
	Logger::debug("PLAYER X ADDRESS= %p\n", &(playerX));
	Logger::debug("PLAYER LOCATION = (%f, %f, %f)\n", playerX, playerY, playerZ);
	Logger::debug("TARGET LOCATION = (%f, %f, %f)\n", x, y, z);
	Logger::debug("====TARGETS LIST====\n");
	float* targetX_ptr = (float*)(*(long long*)(*(long long*)(targetBaseHandle + 0x190) + 0x68) + 0x70);

	switch (teleportType)
	{
	case 1:
		teleportPlayerToTarget(playerX_ptr, targetX_ptr);
		break;
	case 2:
		teleportTargetToPlayer(playerX_ptr);
		break;
	case 3:
		recordTargetsIndex = 0;
		replayTargetIndex = 0;
		for (int i = 0; i <= MAX_NUM_TARGETS - 1; i ++)
		{
			targets[i] = -1;
		}
	case 4:
		teleportList(playerX_ptr);
	default: ;
	}

	int forwardIndex = nextTarget();
	if (forwardIndex != -1)
	{
		Logger::debug("Forwarding replayTargetIndex...");
		replayTargetIndex = forwardIndex;
	}
}

void TargetNpcInfo::addTarget(long long value)
{
	long long* foo = std::find(std::begin(targets), std::end(targets), value);
	if (foo != std::end(targets)) return;
	targets[recordTargetsIndex] = value;
	recordTargetsIndex++;
	recordTargetsIndex %= MAX_NUM_TARGETS;
}

void TargetNpcInfo::teleportList(float* playerX_ptr)
{
	if (targets[replayTargetIndex] == -1)
	{
		int nextTargetIndex = nextTarget();
		if (nextTargetIndex == -1) return;
		replayTargetIndex = nextTargetIndex;
	}
	uint32_t replayTargetHP = getHP(targets[replayTargetIndex]);
	if (replayTargetHP == 0)
	{
		targets[replayTargetIndex] = -1;
		int nextTargetIndex = nextTarget();
		if (nextTargetIndex == -1) return;
		replayTargetIndex = nextTargetIndex;
	}

	float* replayTargetX_ptr = (float*)(*(long long*)(*(long long*)(targets[replayTargetIndex] + 0x190) + 0x68) + 0x70);
	for (long long target : targets)
	{
		Logger::debug("Target Pointer = %lld ", target);
		if (target != -1)
		{
			uint32_t targetHP = getHP(target);
			Logger::debug("Target HP = %d ", targetHP);
		}
		Logger::debug("Record index = %d", recordTargetsIndex);
		Logger::debug("Replay index = %d", replayTargetIndex);
		Logger::debug("\n");
	}

	teleportPlayerToTarget(playerX_ptr, replayTargetX_ptr);
}

int TargetNpcInfo::nextTarget()
{
	int count = 1;
	int forwardIndex = replayTargetIndex;
	do
	{
		forwardIndex++;
		forwardIndex %= MAX_NUM_TARGETS;
		count++;
	}
	while ((targets[forwardIndex] == -1 || getHP(targets[forwardIndex]) == 0) && count <= MAX_NUM_TARGETS);

	if (targets[forwardIndex] == -1)
		return -1;
	return forwardIndex;
}

uint32_t TargetNpcInfo::getHP(long long target)
{
	uint32_t targetHP = *(uint32_t*)(*(long long*)(*(long long*)(target + 0x190) + 0x0) + 0x138);
	return targetHP;
}