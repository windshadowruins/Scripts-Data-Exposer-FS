#include "pch.h"
#include <cstdio>

#include "Logger.h"
#include "target/TargetNpcInfo.h"

extern TargetNpcInfo* targetNpcInfo;

extern "C" {
    void setStructure(long long value) {
        float* targetX_ptr = (float*)(*(long long*)(*(long long*)(value + 0x190) + 0x68) + 0x70);

        try
        {
            float x = *targetX_ptr;
            float y = *(targetX_ptr + 1);
            float z = *(targetX_ptr + 2);
            targetNpcInfo->baseHandle = value;
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
