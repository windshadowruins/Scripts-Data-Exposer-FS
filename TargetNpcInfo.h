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

	void teleportTo()
	{
		if (x == 42 && y == 42 && z == 42)
			return;

	}
};
