#include "pch.h"

extern float rootMotionReductionFactor;
extern "C" {
    float getRootMotionReductionFactor(intptr_t start) {
        // std::random_device rd;
        // std::mt19937 gen(rd());
        // std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        // float randomFloatStart = dist(gen);
        // float randomFloatEnd = dist(gen);
    	float* startReductionPtr = (float*) start;
        float* endReductionPtr = startReductionPtr + 1;
        *startReductionPtr = 0.5;
        *endReductionPtr = 0.5;
        return rootMotionReductionFactor;
    }
}
