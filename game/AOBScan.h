#pragma once
#include "../include/mem/mem.h"
#include "../include/mem/pattern.h"
#include "../include/mem/simd_scanner.h"
#include "ProcessData.h"
#include "common/AOBScanCore.h"

extern void GetText()
{
    const char textStrMatch[] = ".text"; // refused to work with char* string

    const auto pattern = mem::pattern(textStrMatch, ".....");
    const auto region = mem::region(PROCESS_INFO.mInfo.lpBaseOfDll, PROCESS_INFO.mInfo.SizeOfImage);
    uint8_t* result = mem::simd_scanner(pattern).scan(region).any();

    text_size = *reinterpret_cast<uint32_t*>(result + 0x10);
    text = result + *reinterpret_cast<uint32_t*>(result + 0x14);
};

extern void ScanAndAssignAddresses() 
{
    const char* gameDataManAOB = "48 8b 05 ?? ?? ?? ?? 48 85 c0 74 05 48 8b 40 58 c3 c3";
    const char* virtualMemoryFlagAOB = "48 8b 3d ?? ?? ?? ?? 48 85 ff 74 ?? 48 8b 49";
    const char* soloParamRepositoryAOB = "8b da 4c 8b f9 45 33 ed 45 33 e4 48 8b 0d";
    const char* worldChrManAOB = "48 8b 05 ?? ?? ?? ?? 48 85 c0 74 0f 48 39 88";


    const char* hksEnvAOB = "48 8b c4 55 56 57 41 54 41 55 41 56 41 57 48 8d 68 a1 48 81 ec e0 00 00 00 48 c7 45 b7 fe ff ff ff";
    const char* hksActAOB = "48 8b c4 55 56 57 41 56 41 57 48 8d 68 a8 48 81 ec 30 01 00 00 48 c7 44 24 50 fe ff ff ff";
    const char* hks_lua_typeAOB = "41 8b 10 b8 06 00 00 00 83 e2 0f 8d 4a f7 83 f9 01 0f 47 c2 48 83 c4 30 5b c3";
    const char* hksHasParamIntOutAOB = "48 89 5c 24 08 48 89 74 24 10 57 48 83 ec 20 41 8b d8 8b fa 44 8b c2 48 8b f1 48 8b d1 48 8d 4c 24 48 e8 ?? ?? ?? ?? 80 38 00";
    const char* hks_luaL_checkintAOB = "8b d6 48 8b cf e8 ?? ?? ?? ?? 8b c3 48 8b 5c 24 40 48 8b 74 24 48 48 83 c4 30 5f c3";
    const char* hks_luaL_checknumberAOB = "8b d7 48 8b cb e8 ?? ?? ?? ?? 0f 28 f0 0f 57 c9 0f 2e f1 75 ?? 8b d7 48 8b cb";
    const char* hks_luaL_checklstringAOB = "4c 8b c3 8b d6 48 8b cf e8 ?? ?? ?? ?? 48 8b d8 48 85 c0 75";
    const char* getEventFlagAOB = "44 8b da 33 d2 41 8b c3 41 f7 f0 4c 8b d1 45 33 c9 44 0f af c0 45 2b d8";
    const char* setEventFlagAOB = "44 8b d2 33 d2 41 8b c2 41 f7 f1 41 8b d8 4c 8b d9";
    const char* replaceItemAOB = "40 57 48 83 ec 40 48 c7 44 24 30 fe ff ff ff 48 89 5c 24 60 48 89 74 24 68 41 0f b6 f1 41 8b d8 48 8b f9 81 e2 ff ff ff 0f";
    const char* getParamResCapAOB = "48 63 d2 48 8d 04 d2 44 3b 84 c1 80 00 00 00";
    const char* getChrInsFromHandleAOB = "48 83 ec 28 e8 ?? ?? ?? ?? 48 85 c0 74 ?? 48 8b 00 48 83 c4 28 c3 48 83 c4 28 c3";


    //Globals
    //VirtualMemoryFlag = (void **)getAbsoluteAddress(process, 0x3cdf238);

    VirtualMemoryFlag = AOBScanBase(virtualMemoryFlagAOB);
    SoloParamRepository = AOBScanBase(soloParamRepositoryAOB, 18, 14);
    WorldChrMan = AOBScanBase(worldChrManAOB);

    //Functions
    //hksEnv = (int(*)(void**, int, HksState*))getAbsoluteAddressFunc(process, 0x040de30);
    //hksAct = (void(*)(void**, int, HksState*))getAbsoluteAddressFunc(process, 0x040a1e0);
    //hks_lua_type = (int(*)(HksState*, int))getAbsoluteAddressFunc(process, 0x149e6a0);
    //hksHasParamIntOut = (bool* (*)(bool*, HksState*, int))getAbsoluteAddressFunc(process, 0x13f26f0);
    //hks_luaL_checkint = (int(*)(HksState*, int))getAbsoluteAddressFunc(process, 0x14A32C0);
    //hks_luaL_checkoptint = (int(*)(HksState*, int, int))getAbsoluteAddressFunc(process, 0xbce940);
    //hks_luaL_checknumber = (float(*)(HksState*, int))getAbsoluteAddressFunc(process, 0x14a3370);
    //hks_luaL_checklstring = (char* (*)(HksState*, int, int*))getAbsoluteAddressFunc(process, 0x1497180);
    //getEventFlag = (bool (*)(void*, unsigned int))getAbsoluteAddressFunc(process, 0x05edc20);
    //setEventFlag = (void (*)(void*, unsigned int, int))getAbsoluteAddressFunc(process, 0x05ee410);

    hksEnv = (int(*)(void**, int, HksState*))AOBScanCode(hksEnvAOB);
    replacedHksEnv = hksEnv;
    hksAct = (void(*)(void**, int, HksState*))AOBScanCode(hksActAOB);
    replacedHksAct = hksAct;
    hks_lua_type = (int(*)(HksState*, int))AOBScanCode(hks_lua_typeAOB, -257);
    hksHasParamIntOut = (bool* (*)(bool*, HksState*, int))AOBScanCodeCall(hksHasParamIntOutAOB, 34, 34);
    hks_luaL_checkint = (int(*)(HksState*, int))AOBScanCode(hks_luaL_checkintAOB, -123);
    hks_luaL_checknumber = (float(*)(HksState*, int))AOBScanCode(hks_luaL_checknumberAOB, -86);
    hks_luaL_checklstring = (char* (*)(HksState*, int, int*))AOBScanCode(hks_luaL_checklstringAOB, -89);
    getEventFlag = (bool (*)(void*, unsigned int))AOBScanCode(getEventFlagAOB, -4);
    setEventFlag = (void (*)(void*, unsigned int, int))AOBScanCode(setEventFlagAOB, -9);
    replaceItem = (char (*)(void*, int, int, char))AOBScanCode(replaceItemAOB, 0);
    getParamResCap = (void* (*)(void*, int, int))AOBScanCode(getParamResCapAOB, -8);
    getChrInsFromHandle = (void* (*)(void*, uint64_t*))AOBScanCode(getChrInsFromHandleAOB, 0);
}