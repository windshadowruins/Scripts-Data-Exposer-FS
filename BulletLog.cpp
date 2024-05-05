#include "pch.h"

#include "bullet/BulletLog.h"

BulletLog::BulletLog(intptr_t processBaseHandle): numBullets(0), processBaseHandle(processBaseHandle)
{
}

void BulletLog::add(BulletInfo bullet)
{
	bullets.push_back(bullet);
	numBullets = bullets.size();
}
