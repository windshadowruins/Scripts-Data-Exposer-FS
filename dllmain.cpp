// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <Windows.h>
#include <iostream>
#include <vector>
#include <Psapi.h>
#include "game/HksState.h"
#include "include/MinHook.h"
#include "Logger.h"
#include "game/ProcessData.h"
#include "game/AOBScan.h"
#include "world/WorldInfo.h"
#include "target/TargetNpcPatch.h"
#include "bullet/BulletPatch.h"
#include "tae/rootMotionReduction/RootMotionReductionPatch.h"
#include "include/ExposerConfig.h"
#include "include/Logger.h"
#include "game/ProcessData.h"
#include "game/AOBScan.h"
#include "tae/rootMotionReduction/RootMotionReductionPatch.h"


#if _WIN64
#pragma comment(lib, "libMinHook-x64-v141-md.lib")
#else
#pragma comment(lib, "libMinHook-x86-v141-md.lib")
#endif

#define OPEN_CONSOLE_ON_START 0

static void initAddresses()
{
    GetText();
    ScanAndAssignAddresses();
}


bool createHook(LPVOID pTarget, LPVOID pDetour, LPVOID* ppOriginal)
{
    int mhStatus = MH_CreateHook(pTarget, pDetour, ppOriginal);
    if (mhStatus != MH_OK)
    {
        Logger::log("MinHook CreateHook error creating hook (%d)", mhStatus);
        return false;
    }
    return true;
}

void initTargetHooks()
{
	const unsigned char targetStructureAOB[] = { 0x48, 0x8B, 0x48, 0x08, 0x49, 0x89, 0x8D };
    const char* targetStructureMask = ".......";
    // 48 8B 48 08 49 89 8D
    // for CE scanning
    Logger::info("About to scan for target structure...\n");
    void* targetStructureMoveInstruction = AOBScanAddress(targetStructureAOB, targetStructureMask);

    Logger::info("Target structure code location: %p\n", targetStructureMoveInstruction);
    // Insert Jump
    unsigned char* jumpAddress = reinterpret_cast<unsigned char*>(targetStructureMoveInstruction);
    unsigned char asmCode[] = {
        0x48, 0xB9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xD1, 0x90, 0x90,
    };

    SIZE_T allocationSize = 4096;

    void* allocatedMemory = target_decorator;

	DWORD oldProtect;
    VirtualProtect(jumpAddress, sizeof(asmCode), PAGE_EXECUTE_READWRITE, &oldProtect);

    unsigned char* masmRoutineAddress = reinterpret_cast<unsigned char*>(allocatedMemory);

	unsigned char* start_of_next_instruction = jumpAddress + SIZE_OF_CALL_INSTRUCTION;
    // int relativeOffset = reinterpret_cast<uintptr_t>(masmRoutineAddress) - reinterpret_cast<uintptr_t>(start_of_next_instruction);
    Logger::info("Pointer to Jump is %p \n", jumpAddress);
    Logger::info("Pointer to start of after-Jump instruction is %p \n", (void*) start_of_next_instruction);
    // std::cout << "Offset to Decorator from jump is " << relativeOffset << std::endl;
    Logger::info("Pointer to Decorator is %p \n", (void*) masmRoutineAddress);
    // Logger::debug("Double checking offset calculation %p \n", (void*) (reinterpret_cast<uintptr_t>(start_of_next_instruction) + relativeOffset));


    // Patch the offset into the call instruction
	for (int i = 0; i <= ADDRESS_SIZE_IN_BYTES - 1; ++i) {
        asmCode[i + 2] = (reinterpret_cast<uintptr_t>(allocatedMemory) >> (i * 8)) & 0xFF;
    }

	for (DWORD fragment : asmCode)
    {
        Logger::info("%02X ", fragment);
    }
    Logger::info("\n");

    // Copy assembly code to the target address
    memcpy(jumpAddress, asmCode, sizeof(asmCode));

    // Restore the original memory protection
    VirtualProtect(jumpAddress, sizeof(asmCode), oldProtect, &oldProtect);

    targetNpcInfo = new TargetNpcInfo(getProcessBase());
    targetNpcInfo -> x = DEFAULT_COORDINATES;
    targetNpcInfo -> y = DEFAULT_COORDINATES;
    targetNpcInfo -> z = DEFAULT_COORDINATES;

    Logger::info("Target NPC info at : %p\n", targetNpcInfo);
}

void initCreateBulletHook()
{
    // F3 0F 11 B3 14 0B 00 00 F3 0F 11 B3 18 0B 00 00 0F 28 74 24 40 48 83 C4 50 5B C3
    // for CE scanning

    // 48 B9 30 4F 34 3C FF
    // For searching hook

    const unsigned char createBulletAOB[] = {0xF3, 0x0F, 0x11, 0xB3, 0x14, 0x0B, 0x00, 0x00, 0xF3, 0x0F, 0x11, 0xB3, 0x18, 0x0B, 0x00, 0x00, 0x0F, 0x28, 0x74, 0x24, 0x40, 0x48, 0x83, 0xC4, 0x50, 0x5B, 0xC3 };
    const char* createBulletMask = "...........................";
    void* createBulletInvariant = AOBScanAddress(createBulletAOB, createBulletMask);
    intptr_t createBulletInvariantAddress = (intptr_t)createBulletInvariant;
    // mov rcx, <address>
    // nop...
	// unsigned char asmCode[] = {
	//     0x48, 0xB9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xD1, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
	//     0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
 //    };
	unsigned char asmCode[] = {
	    0x48, 0xB9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xD1, 0x90, 0x90, 0x90, 0x90, };
    unsigned char* jumpAddress = reinterpret_cast<unsigned char*>(createBulletInvariant);

    void* allocatedMemory = bullet_decorator;

    DWORD oldProtect;
    VirtualProtect(jumpAddress, sizeof(asmCode), PAGE_EXECUTE_READWRITE, &oldProtect);

    unsigned char* masmRoutineAddress = reinterpret_cast<unsigned char*>(allocatedMemory);

    unsigned char* start_of_next_instruction = jumpAddress + SIZE_OF_CALL_INSTRUCTION;
    Logger::info("Pointer to Jump is %p \n", jumpAddress);
    Logger::info("Pointer to start of after-Jump instruction is %p \n", (void*)start_of_next_instruction);
    Logger::info("Pointer to Decorator is %p \n", (void*)masmRoutineAddress);

    // Patch the address into the call instruction
    for (int i = 0; i <= ADDRESS_SIZE_IN_BYTES - 1; ++i) {
        asmCode[i + 2] = (reinterpret_cast<uintptr_t>(allocatedMemory) >> (i * 8)) & 0xFF;
    }

    for (DWORD fragment : asmCode)
    {
        Logger::info("%02X ", fragment);
    }
    Logger::info("\n");

    // Copy assembly code to the target address
    memcpy(jumpAddress, asmCode, sizeof(asmCode));

    // Restore the original memory protection
    VirtualProtect(jumpAddress, sizeof(asmCode), oldProtect, &oldProtect);

    bulletLog = new BulletLog(getProcessBase());
    Logger::info("Bullet info at : %p\n", bulletLog);
}

void initCharacterListHook()
{
    // 0F 10 00 0F 11 44 24 70 0F 10 48 10 0F 11 4D 80 48 83 3D
    // for CE scanning
    const unsigned char characterListAOB[] = { 0x0F, 0x10, 0x00, 0x0F, 0x11, 0x44, 0x24, 0x70, 0x0F, 0x10, 0x48, 0x10, 0x0F, 0x11, 0x4D, 0x80, 0x48, 0x83, 0x3D };
    const char* characterListMask = "...................";
    Logger::info("About to scan for character List...\n");
    void* worldChrManInvariant = AOBScanAddress(characterListAOB, characterListMask);
    int64_t worldChrManInvariantAddress = (int64_t) worldChrManInvariant;
    int worldChrManInvariantAddressOffset19 = *(int *)(worldChrManInvariantAddress + 19);
    worldChrManAddress = (worldChrManInvariantAddress + 24 + worldChrManInvariantAddressOffset19);
	Logger::info("WORLD_CHR_MAIN = %p\n", reinterpret_cast<unsigned char*>(worldChrManInvariantAddress));
}

void initRootMotionReductionHook()
{
    // f3 41 0f 10 70 04 f3 41 0f 5c 30 0f 29 7c 24 20
    // for CE scanning unmodified code

    // 48 8d 44 24 50 f3 41 0f 10 70 04 f3 41 0f 5c 30 0f 29 7c 24 20
    // for CE scanning unmodified code

    // f3 0f 5c 00 f3 0f 5c 10 0f 29 74 24 30
    // for CE scanning after
    const unsigned char rmrAccessAOB[] = { 0x48, 0x8d, 0x44, 0x24, 0x50, 0xf3, 0x41, 0x0f, 0x10, 0x70, 0x04, 0xf3, 0x41, 0x0f, 0x5c, 0x30, 0x0f, 0x29, 0x7c, 0x24, 0x20 };
    const char* rmrAccessMask = "................";
    Logger::info("About to scan for Root Motion Reduction access code...\n");
    void* rmrAccessInvariant = AOBScanAddress(rmrAccessAOB, rmrAccessMask);
    int64_t rmrAccessInvariantAddress = (int64_t)rmrAccessInvariant;

    unsigned char asmCode[] = {
    0x49, 0x89, 0xE2, 0x48, 0xB9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xD1, 0x90, };
    unsigned char* jumpAddress = reinterpret_cast<unsigned char*>(rmrAccessInvariant);

    DWORD oldProtect;
    VirtualProtect(jumpAddress, sizeof(asmCode), PAGE_EXECUTE_READWRITE, &oldProtect);
    void* allocatedMemory = rmr_decorator;
    unsigned char* start_of_next_instruction = jumpAddress + SIZE_OF_CALL_INSTRUCTION;
    unsigned char* masmRoutineAddress = reinterpret_cast<unsigned char*>(allocatedMemory);
    Logger::info("Pointer to Jump from is %p \n", jumpAddress);
    // Logger::info("Pointer to start of after-Jump instruction is %p \n", (void*)start_of_next_instruction);
    Logger::info("Pointer to Decorator is %p \n", (void*)masmRoutineAddress);
    // Logger::info("Offset is %p \n", (void*)masmRoutineAddress);
    for (int i = 0; i <= ADDRESS_SIZE_IN_BYTES - 1; ++i) {
        asmCode[i + 5] = (reinterpret_cast<uintptr_t>(allocatedMemory) >> (i * 8)) & 0xFF;
    }
    for (DWORD fragment : asmCode)
    {
        Logger::info("%02X ", fragment);
    }
    Logger::info("\n");

    // Copy assembly code to the target address
    memcpy(jumpAddress, asmCode, sizeof(asmCode));
    VirtualProtect(jumpAddress, sizeof(asmCode), oldProtect, &oldProtect);

    taeEditor = new TaeEditor();
    Logger::info("Temp Factor pointer is at %p \n", (void*) taeEditor -> rootMotionReductionFactor);
}

void initHooks() 
{
    createHook(replacedHksEnv, &envHookFunc, (void**)&hksEnv);
    createHook(replacedHksAct, &actHookFunc, (void**)&hksAct);

    initRootMotionReductionHook();
    initTargetHooks();
    initCreateBulletHook();
    initCharacterListHook();
    MH_EnableHook(MH_ALL_HOOKS);
}

void onAttach()
{
    if (OPEN_CONSOLE_ON_START && GetConsoleWindow() == NULL && buildType == BuildType::DEBUG) 
    {
        AllocConsole();
        freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
        Logger::log("Created Scripts-Data-Exposer-FS Console");

    }

    Logger::log("Start onAttach");

    initBase();

    int mhStatus = MH_Initialize();
    if (mhStatus != MH_OK) 
    {
        Logger::log("MinHok Initialize error " + mhStatus);
        return;
    }

    initAddresses();

    initHooks();

    Logger::log("Finished onAttach");
}

void onDetach()
{
    delete targetNpcInfo;
    delete taeEditor;
    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        onAttach();
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        onDetach();
        break;
    }
    return TRUE;
}

