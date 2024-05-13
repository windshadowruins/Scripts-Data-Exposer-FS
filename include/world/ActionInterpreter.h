#pragma once

#include <Windows.h>
#include <Psapi.h>
#include "../game/ProcessStructs.h"

void interpretTeleport(void** chrInsPtr, HksState* hksState);
void interpretTeleportToBullet(void** chrInsPtr, HksState* hksState);
void interpretTae(void** chrInsPtr, HksState* hksState);

