#include "pch.h"
#include "../extensions/ActionInterpreter.h"

#include "common/CoordinatePointers.h"
#include "world/WorldInfo.h"

extern bool hksHasParamInt(HksState* hksState, int paramIndex);
extern int (*hks_luaL_checkint)(HksState* hksState, int paramIndex);

void interpretTeleport(void** chrInsPtr, HksState* hksState)
{
    if (!hksHasParamInt(hksState, 2)) return;
    intptr_t chrIns = (intptr_t)*chrInsPtr;
    CoordinatePointers playerCoordinatePointers = targetNpcInfo->updatePlayerCoordinates(chrIns);
    
    int teleportType = hks_luaL_checkint(hksState, 2);
    targetNpcInfo->teleport(teleportType, playerCoordinatePointers);
}

void interpretTeleportToBullet(void** chrInsPtr, HksState* hksState)
{
    if (!hksHasParamInt(hksState, 2)) return;
    intptr_t chrIns = (intptr_t)*chrInsPtr;
    CoordinatePointers playerCoordinatePointers = targetNpcInfo->updatePlayerCoordinates(chrIns);
    
    int targetBulletID = hks_luaL_checkint(hksState, 2);
    bulletLog->teleport(targetBulletID, playerCoordinatePointers);
}

void interpretTae(void** chrInsPtr, HksState* hksState)
{
    if (!hksHasParamInt(hksState, 2)) return;
    float rootMotionReductionFactor = hks_luaL_checkint(hksState, 2) / 1000.0f;
    taeEditor->updateRootMotion(rootMotionReductionFactor);
}
