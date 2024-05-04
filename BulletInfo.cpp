#include "pch.h"
#include "bullet/BulletInfo.h"

BulletInfo::BulletInfo(intptr_t processBase)
{
	processBaseAddress = processBase;
	bulletBaseHandle = -1;
	x = y = z = 42;
}

CoordinatePointers BulletInfo::position() const
{
	float* bulletX = (float*)(bulletBaseHandle + 0x10);
	return CoordinatePointers{ bulletX, bulletX + 1, bulletX + 2 };
}
