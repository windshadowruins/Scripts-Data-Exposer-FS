#include "pch.h"
#include "../game/ProcessStructs.h"
#include "../../include/extensions/EnvironmentInterpreter.h"

#include "common/CoordinatePointers.h"
#include "mem/prot_flags.h"
#include "world/WorldInfo.h"

extern bool hksHasParamInt(HksState* hksState, int paramIndex);
extern int (*hks_luaL_checkint)(HksState* hksState, int paramIndex);

int interpretEnv(void** chrInsPtr, HksState* hksState)
{
    if (!hksHasParamInt(hksState, 2))
        return -1;
    intptr_t chrIns = (intptr_t)*chrInsPtr;
    int positionIndex = hks_luaL_checkint(hksState, 2);
    if (positionIndex == 4)
    {
        CoordinatePointers playerCoordinatePointers = targetNpcInfo->updatePlayerCoordinates(chrIns);
        float actualDistance = targetNpcInfo->distanceToTarget(playerCoordinatePointers);
        return *((intptr_t*)&actualDistance) & 0xFFFFFFFF;
    }
    float coordinate = targetNpcInfo->getCoordinate(static_cast<TargetNpcPosition>(positionIndex));
    if (coordinate == mem::prot_flags::INVALID) return coordinate;
    return *((intptr_t*)&coordinate) & 0xFFFFFFFF;
}