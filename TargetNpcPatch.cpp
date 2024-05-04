#include "pch.h"
#include <cstdio>

#include "Logger.h"
#include "target/TargetNpcInfo.h"
#include "bullet/BulletInfo.h"

extern TargetNpcInfo* targetNpcInfo;
extern BulletInfo* bulletInfo;

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

extern "C" {
    void setBulletStructure(intptr_t value) {
        bulletInfo->bulletBaseHandle = value;
    }
}
