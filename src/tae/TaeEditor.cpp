#include "pch.h"
#include "tae/TaeEditor.h"

#include "world/WorldInfo.h"

TaeEditor::TaeEditor()
{
	rootMotionReductionFactor = new float;
	*rootMotionReductionFactor = 0.;
}

void TaeEditor::updateRootMotion(float motionReductionFactor) const
{
	*rootMotionReductionFactor = motionReductionFactor;
}

TaeEditor::~TaeEditor()
{
	delete rootMotionReductionFactor;
}
