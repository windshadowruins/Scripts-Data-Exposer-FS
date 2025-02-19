#include "pch.h"

#include "bullet/BulletLog.h"

#include "Logger.h"
#include "common/CoordinatePointers.h"

BulletLog::BulletLog(intptr_t processBaseHandle): numBullets(0), processBaseHandle(processBaseHandle)
{
}

void BulletLog::add(BulletInfo bullet)
{
	bullets.push_back(bullet);
	numBullets = bullets.size();
}

void BulletLog::teleport(int targetBulletID, const CoordinatePointers& playerCoordinatePointers) const
{
	Logger::debug("BULLET LIST\n====================\n");
	Logger::debug("Target Bullet ID is %d \n", targetBulletID);
	Logger::debug("Player position = (%f, %f, %f)\n", *playerCoordinatePointers.x, *playerCoordinatePointers.y, *playerCoordinatePointers.z);

	for (BulletInfo bullet : bullets)
	{
		CoordinatePointers bulletPosition = bullet.position();
		Logger::debug("Bullet %d = (%f, %f, %f)\n", bullet.bulletID, *bulletPosition.x, *bulletPosition.y, *bulletPosition.z);
	}
	Logger::debug("\n\n");
	auto it = std::find_if(bullets.rbegin(), bullets.rend(), [targetBulletID](BulletInfo b)
	{
		return b.bulletID == targetBulletID;
	});
	if (it == bullets.rend())
	{
		Logger::debug("No suitable bullet with ID %d found.", targetBulletID);
		return;
	}
	Logger::debug("Teleporting to bullet ID %d", it->bulletID);
	teleportPlayerTo(playerCoordinatePointers.x, it->position().x);
}

void BulletLog::teleportPlayerTo(float* from, float* to)
{
	memcpy(from, to, 12);
}

