#include "pch.h"
#include "target/TargetNpcInfo.h"

#include <iostream>
#include <ostream>

#include "Logger.h"
#include "mem/prot_flags.h"
#include "game/CoordinatePointers.h"
#include "world/WorldInfo.h"
#include "target/TargetNpcPosition.h"

TargetNpcInfo::TargetNpcInfo(intptr_t processBaseAddress)
{
	this->processBaseAddress = processBaseAddress;
	targetBaseHandle = -1;
}

bool TargetNpcInfo::exists() const
{
	return targetBaseHandle != -1;
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

CoordinatePointers TargetNpcInfo::updatePlayerCoordinates(intptr_t chrIns)
{
	float* playerX_ptr = (float*)(*(intptr_t*)(*(intptr_t*)(chrIns + 0x190) + 0x68) + 0x70);
	float* playerY_ptr = (float*)(*(intptr_t*)(*(intptr_t*)(chrIns + 0x190) + 0x68) + 0x74);
	float* playerZ_ptr = (float*)(*(intptr_t*)(*(intptr_t*)(chrIns + 0x190) + 0x68) + 0x78);

	playerX = *playerX_ptr;
	playerY = *playerY_ptr;
	playerZ = *playerZ_ptr;

	return CoordinatePointers{ playerX_ptr, playerY_ptr, playerZ_ptr };
}

void TargetNpcInfo::teleportTargetToPlayer(float* playerX_ptr) const
{
	std::vector<intptr_t> allCharacters = allLoadedCharacters(processBaseAddress);
	if (!isValidCharacter(targetBaseHandle, allCharacters)) return;
	float* targetX_ptr = (float*)(*(intptr_t*)(*(intptr_t*)(targetBaseHandle + 0x190) + 0x68) + 0x70);
	memcpy(targetX_ptr, playerX_ptr, 12);
}

void TargetNpcInfo::teleportPlayerToTarget(float* playerX_ptr) const
{
	std::vector<intptr_t> allCharacters = allLoadedCharacters(processBaseAddress);
	if (!isValidCharacter(targetBaseHandle, allCharacters)) return;
	teleportPlayerTo(playerX_ptr, targetBaseHandle);
}

void TargetNpcInfo::teleportPlayerTo(float* playerX_ptr, intptr_t destinationX_address)
{
	float* destinationX_ptr = (float*)(*(intptr_t*)(*(intptr_t*)(destinationX_address + 0x190) + 0x68) + 0x70);
	memcpy(playerX_ptr, destinationX_ptr, 12);
}

void TargetNpcInfo::speedUpEnemy() const
{
	float* targetSpeedModifier_ptr = (float*)(*(intptr_t*)(*(intptr_t*)(targetBaseHandle + 0x190) + 0x28) + 0x17C8);
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

bool TargetNpcInfo::isLoaded(intptr_t characterBaseHandle, std::vector<intptr_t>& allCharacters)
{
	std::vector<intptr_t>::iterator iter = std::find(allCharacters.begin(), allCharacters.end(), characterBaseHandle);
	return iter != allCharacters.end();
}

bool TargetNpcInfo::isValidCharacter(intptr_t characterBaseHandle, std::vector<intptr_t>& allCharacters)
{
	if (characterBaseHandle == -1) return false;
	bool isOnMap = isLoaded(characterBaseHandle, allCharacters);
	return isOnMap && isAlive(characterBaseHandle);
}

bool TargetNpcInfo::isAlive(intptr_t characterBaseHandle)
{
	return getHP(characterBaseHandle) > 0;
}

void TargetNpcInfo::teleport(int teleportType, const CoordinatePointers& playerCoordinatePointers)
{
	float* playerX_ptr = playerCoordinatePointers.x;

	// if (x == 42 && y == 42 && z == 42) return;
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
	case 3:
		teleportPlayerToList(playerX_ptr);
		break;
	case 4:
		resetTeleportList();
		break;
	default: ;
	}
}

void TargetNpcInfo::addTarget(intptr_t value)
{
	intptr_t* foo = std::find(std::begin(targets), std::end(targets), value);
	if (foo != std::end(targets)) return;
	targets[recordTargetsIndex] = value;
	recordTargetsIndex++;
	recordTargetsIndex %= MAX_NUM_TARGETS;
}

void TargetNpcInfo::teleportPlayerToList(float* playerX_ptr)
{
	Logger::debug("Record index = %d", recordTargetsIndex);
	Logger::debug("Replay index = %d", replayTargetIndex);
	Logger::log("\n");
	int temp = replayTargetIndex;
	int count = 0;
	std::vector<intptr_t> allCharacters = allLoadedCharacters(processBaseAddress);

	while (!isValidCharacter(targets[temp], allCharacters) && count < MAX_NUM_TARGETS)
	{
		temp++;
		temp %= MAX_NUM_TARGETS;
		count++;
	}

	replayTargetIndex = temp;
	if (count >= MAX_NUM_TARGETS)
	{
		Logger::debug("No suitable targets...not teleporting.\n");
		return;
	}
	// Logger::debug("====TARGETS LIST====\n");
	// for (long long target : targets)
	// {
	// 	Logger::debug("Target Pointer = %lld ", target);
	// 	if (target != -1)
	// 	{
	// 		uint32_t targetHP = getHP(target);
	// 		Logger::debug("Target HP = %d ", targetHP);
	// 	}
	// 	Logger::debug("Record index = %d", recordTargetsIndex);
	// 	Logger::debug("Replay index = %d", replayTargetIndex);
	// 	Logger::debug("\n");
	// }

	teleportPlayerTo(playerX_ptr, targets[replayTargetIndex]);
	replayTargetIndex++;
	replayTargetIndex %= MAX_NUM_TARGETS;
}

int TargetNpcInfo::getHP(intptr_t target)
{
	uint32_t targetHP = *(uint32_t*)(*(intptr_t*)(*(intptr_t*)(target + 0x190) + 0x0) + 0x138);
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