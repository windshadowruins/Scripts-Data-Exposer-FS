#include "pch.h"
#include "bullet/BulletInfo.h"

#include "common/CoordinatePointers.h"

BulletInfo::BulletInfo(intptr_t bulletBaseAddress): bulletBaseHandle(bulletBaseAddress)
{
	x = y = z = 42;
	bulletID = *(int32_t*)(bulletBaseHandle + 0x4B4);
}

// 4B4 = Bullet ID
// 4C0 = Shooter ID
CoordinatePointers BulletInfo::position() const
{
	float* bulletX = (float*)(bulletBaseHandle + 0x10);
	return CoordinatePointers{ bulletX, bulletX + 1, bulletX + 2 };
}
