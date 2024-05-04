#pragma once
#include "pch.h"

#include "../target/CoordinatePointers.h"

struct BulletInfo
{
	intptr_t processBaseAddress;
	intptr_t bulletBaseHandle;
	float x, y, z;

	BulletInfo(intptr_t processBase);
	CoordinatePointers position() const;
};
