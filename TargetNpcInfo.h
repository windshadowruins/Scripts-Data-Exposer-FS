#pragma once

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

	Position getCoordinates()
	{
		return { x, y, z };
	}

	float** updatePlayerCoordinates(intptr_t chrIns)
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

	void teleportTo(float* playerX_ptr)
	{
		float* targetX_ptr = (float*)(*(long long*)(*(long long*)(baseHandle + 0x190) + 0x68) + 0x70);
		memcpy(targetX_ptr, playerX_ptr, 12);

	}
	void speedUpEnemy()
	{
		float* targetSpeedModifier_ptr = (float*)(*(long long*)(*(long long*)(baseHandle + 0x190) + 0x28) + 0x17C8);
		*targetSpeedModifier_ptr = 25.0;

	}
};
