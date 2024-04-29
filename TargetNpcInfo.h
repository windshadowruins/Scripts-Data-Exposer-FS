#pragma once

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
