#include "pch.h"
#include "rootMotionReductionPatch/RootMotionReductionPatch.h"

#include "world/WorldInfo.h"

extern float rootMotionReductionFactor;

extern "C" {
	float getRootMotionReductionFactor(intptr_t start)
	{
		float* startReductionPtr = (float*)start;
		float* endReductionPtr = startReductionPtr + 1;
		*startReductionPtr = *startReductionPtr + 0.0001;
		*endReductionPtr = *endReductionPtr + 0.0001;
		*tempFactorStorage = *startReductionPtr;
		// *endReductionPtr = 0.5;
		// if (!targetNpcInfo->exists()) return 1;
		// float distance = pow(targetNpcInfo->x - targetNpcInfo->playerX, 2) +
		// 	pow(targetNpcInfo->y - targetNpcInfo->playerY, 2) +
		// 	pow(targetNpcInfo->z - targetNpcInfo->playerZ, 2);
		//
		// *tempFactorStorage = (2500 - distance) / 2500;
		// *tempFactorStorage = randomFloat();
		// return rootMotionReductionFactor
		return 1;
	}
}

float randomFloat()
{
	return (float)(rand()) / (float)(RAND_MAX);
}
