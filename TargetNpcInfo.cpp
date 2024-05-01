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

	float* coordinatePtrs[3] = { playerX_ptr, playerY_ptr, playerZ_ptr };
	return coordinatePtrs;
}

void TargetNpcInfo::teleportTargetToPlayer(float* playerX_ptr) const
{
	float* targetX_ptr = (float*)(*(long long*)(*(long long*)(baseHandle + 0x190) + 0x68) + 0x70);
	memcpy(targetX_ptr, playerX_ptr, 12);

}
void TargetNpcInfo::teleportPlayerToTarget(float* playerX_ptr) const
{
	float* targetX_ptr = (float*)(*(long long*)(*(long long*)(baseHandle + 0x190) + 0x68) + 0x70);
	memcpy(playerX_ptr, targetX_ptr, 12);
}
void TargetNpcInfo::speedUpEnemy() const
{
	float* targetSpeedModifier_ptr = (float*)(*(long long*)(*(long long*)(baseHandle + 0x190) + 0x28) + 0x17C8);
	*targetSpeedModifier_ptr = 25.0;
}
void TargetNpcInfo::teleport(int teleportType, float** playerCoordinatePointers)
{
	float* playerX_ptr = playerCoordinatePointers[0];

	if (x == 42 && y == 42 && z == 42) return;
	Logger::debug("PLAYER X ADDRESS= %p\n", &(playerX));
	Logger::debug("PLAYER LOCATION = (%f, %f, %f)\n", playerX, playerY, playerZ);
	Logger::debug("TARGET LOCATION = (%f, %f, %f)\n", x, y, z);

	switch (teleportType)
	{
	case 1:
		teleportPlayerToTarget(playerX_ptr);
		break;
	case 2:
		teleportTargetToPlayer(playerX_ptr);
		break;
	default:;
	}

}
