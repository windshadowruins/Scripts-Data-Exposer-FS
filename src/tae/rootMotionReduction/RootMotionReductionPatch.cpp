#include "pch.h"
#include "tae/rootMotionReduction/RootMotionReductionPatch.h"
#include "world/WorldInfo.h"

extern "C" {
float getRootMotionReductionFactor(intptr_t start)
{
	float* startReductionPtr = (float*)start;
	float* endReductionPtr = startReductionPtr + 1;
	// *startReductionPtr = *startReductionPtr + 0.001;
	// *endReductionPtr = *endReductionPtr + 0.001;
	// *rootMotionReductionFactor = *startReductionPtr;
	*startReductionPtr = *endReductionPtr = *(taeEditor -> rootMotionReductionFactor);
	return 1;
}
}

float randomFloat()
{
	return (float)(rand()) / (float)(RAND_MAX);
}
