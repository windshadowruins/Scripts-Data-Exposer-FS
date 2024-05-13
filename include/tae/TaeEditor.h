#pragma once

struct TaeEditor
{
	float* rootMotionReductionFactor;

	TaeEditor();
	void updateRootMotion(float motionReductionFactor) const;
	~TaeEditor();
};
