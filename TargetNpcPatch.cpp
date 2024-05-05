#include "pch.h"

#include "Logger.h"
#include "target/TargetNpcInfo.h"

extern TargetNpcInfo* targetNpcInfo;

extern "C" {
    void setStructure(intptr_t value) {
        float* targetX_ptr = (float*)(*(intptr_t*)(*(intptr_t*)(value + 0x190) + 0x68) + 0x70);

        try
        {
            float x = *targetX_ptr;
            float y = *(targetX_ptr + 1);
            float z = *(targetX_ptr + 2);
            targetNpcInfo->targetBaseHandle = value;
            targetNpcInfo->addTarget(value);
            targetNpcInfo->x = x;
            targetNpcInfo->y = y;
            targetNpcInfo->z = z;
        }
        catch (char* e)
        {
            Logger::error("Exception Caught: %s\n", e);
        }
    }
}
