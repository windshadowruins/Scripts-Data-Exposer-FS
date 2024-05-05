#pragma once
#include "pch.h"

#include <vector>

#include "../target/CoordinatePointers.h"

struct BulletInfo
{
	intptr_t bulletBaseHandle;
	float x, y, z;

	BulletInfo(intptr_t bulletBaseAddress);
	CoordinatePointers position() const;
};
