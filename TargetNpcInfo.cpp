#include "pch.h"
#include "target/TargetNpcInfo.h"

#include <iostream>
#include <ostream>

#include "Logger.h"
#include "mem/prot_flags.h"
#include "target/TargetNpcInfoPtr.h"
#include "target/TargetNpcPosition.h"

TargetNpcInfo::TargetNpcInfo(intptr_t processBaseAddress)
{
	this->processBaseAddress = processBaseAddress;
	targetBaseHandle = -1;
}

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

void TargetNpcInfo::teleportPlayerToTarget(float* playerX_ptr)
{
	if (!isValidCharacter(targetBaseHandle)) return;
	float* targetX_ptr = (float*)(*(long long*)(*(long long*)(targetBaseHandle + 0x190) + 0x68) + 0x70);
	memcpy(playerX_ptr, targetX_ptr, 12);
}

void TargetNpcInfo::speedUpEnemy() const
{
	float* targetSpeedModifier_ptr = (float*)(*(long long*)(*(long long*)(targetBaseHandle + 0x190) + 0x28) + 0x17C8);
	*targetSpeedModifier_ptr = 25.0;
}

void TargetNpcInfo::resetTeleportList()
{
	recordTargetsIndex = 0;
	replayTargetIndex = 0;
	for (int i = 0; i <= MAX_NUM_TARGETS - 1; i++)
	{
		targets[i] = -1;
	}
}

bool TargetNpcInfo::isLoaded(intptr_t characterBaseHandle, std::vector<long long> allCharacters)
{
	std::vector<long long>::iterator iter = std::find(allCharacters.begin(), allCharacters.end(), characterBaseHandle);
	return iter != allCharacters.end();
}

bool TargetNpcInfo::isValidCharacter(intptr_t characterBaseHandle)
{
	std::vector<intptr_t> allCharacters = allLoadedCharacters(processBaseAddress);
	bool isOnMap = isLoaded(characterBaseHandle, allCharacters);
	return isOnMap && isAlive(characterBaseHandle);
}

bool TargetNpcInfo::isAlive(intptr_t characterBaseHandle)
{
	return getHP(characterBaseHandle) > 0;
}

void TargetNpcInfo::teleport(int teleportType, float** playerCoordinatePointers)
{
	float* playerX_ptr = playerCoordinatePointers[0];

	// if (x == 42 && y == 42 && z == 42) return;
	Logger::debug("PLAYER X ADDRESS= %p\n", &(playerX));
	Logger::debug("PLAYER LOCATION = (%f, %f, %f)\n", playerX, playerY, playerZ);
	Logger::debug("TARGET LOCATION = (%f, %f, %f)\n", x, y, z);
	if (!isValidCharacter(targetBaseHandle)) return;
	float* targetX_ptr = (float*)(*(long long*)(*(long long*)(targetBaseHandle + 0x190) + 0x68) + 0x70);
	Logger::debug("====TARGETS LIST====\n");

	switch (teleportType)
	{
	case 1:
		teleportPlayerToTarget(playerX_ptr);
		break;
	case 2:
		teleportTargetToPlayer(playerX_ptr);
		break;
	case 3:
		resetTeleportList();
		break;
	case 4:
		teleportList(playerX_ptr);
	default: ;
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
	std::vector<intptr_t> allCharacters = allLoadedCharacters(processBaseAddress);
	if (targets[replayTargetIndex] == -1)
	{
		int nextTargetIndex = nextTarget();
		if (nextTargetIndex == -1) return;
		replayTargetIndex = nextTargetIndex;
	}

	if (! isValidCharacter(targets[replayTargetIndex]))
	{
		Logger::debug("Possible warp, resetting target list...");
		resetTeleportList();
		return;
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

	teleportPlayerToTarget(playerX_ptr);
	int forwardIndex = nextTarget();
	if (forwardIndex != -1)
	{
		Logger::debug("Forwarding replayTargetIndex...");
		replayTargetIndex = forwardIndex;
	}
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

int TargetNpcInfo::getHP(long long target)
{
	uint32_t targetHP = *(uint32_t*)(*(long long*)(*(long long*)(target + 0x190) + 0x0) + 0x138);
	return targetHP;
}

std::vector<intptr_t> TargetNpcInfo::allLoadedCharacters(intptr_t processBase)
{
	int64_t WORLD_CHR_MAN = worldChrManAddress;
	int CHR_ENTRY_LIST_START = 0x1F1B0;
	int CHR_ENTRY_LIST_END = 0x1F1B8;

	intptr_t charactersEnd = *(intptr_t*)(*(intptr_t*)WORLD_CHR_MAN + CHR_ENTRY_LIST_END);
	intptr_t charactersStart = *(intptr_t*)(*(intptr_t*)WORLD_CHR_MAN + CHR_ENTRY_LIST_START);

	intptr_t numLoadedCharacters = (charactersEnd - charactersStart) / 8;
	std::vector<intptr_t> characterAddresses(numLoadedCharacters);
	std::cout << "NUM LOADED = " << numLoadedCharacters << std::endl;
	Logger::debug("%d characters loaded \n", numLoadedCharacters);
	for (intptr_t targetAddress = charactersStart; targetAddress <= charactersEnd - 8; targetAddress += 8)
	{
		// Logger::debug("Character base handle: %p \n", reinterpret_cast<unsigned char*>(targetAddress));
		intptr_t actualAddress = *(intptr_t*)targetAddress;
		characterAddresses.push_back(actualAddress);
		// float* targetSpeedModifier_ptr = (float*)(*(intptr_t*)(*(intptr_t*)(actualAddress + 0x190) + 0x28) + 0x17C8);
		// Logger::debug("Modifier is : %f", *targetSpeedModifier_ptr);
		// *targetSpeedModifier_ptr = 25.0;
	}
	return characterAddresses;
}