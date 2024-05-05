#pragma once
#include "pch.h"

#include <vector>

#include "../game/CoordinatePointers.h"

struct BulletInfo
{
	intptr_t bulletBaseHandle;
	float x, y, z;
	int32_t bulletID;

	BulletInfo(intptr_t bulletBaseAddress);
	CoordinatePointers position() const;
};
