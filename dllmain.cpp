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
#include "target/TargetNpcInfoPtr.h"
#include "target/TargetNpcPatch.h"
#include "include/ExposerConfig.h"
#include "include/Logger.h"
#include "game/ProcessData.h"
#include "game/AOBScan.h"


#if _WIN64
#pragma comment(lib, "libMinHook-x64-v141-md.lib")
#else
#pragma comment(lib, "libMinHook-x86-v141-md.lib")
#endif

#define OPEN_CONSOLE_ON_START 1

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

    void* allocatedMemory = decorator;

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

    targetNpcInfo = new TargetNpcInfo;
    targetNpcInfo -> x = DEFAULT_COORDINATES;
    targetNpcInfo -> y = DEFAULT_COORDINATES;
    targetNpcInfo -> z = DEFAULT_COORDINATES;

    Logger::info("Target NPC info at : %p\n", targetNpcInfo);
}

void initHooks() 
{
    createHook(replacedHksEnv, &envHookFunc, (void**)&hksEnv);
    createHook(replacedHksAct, &actHookFunc, (void**)&hksAct);

    initTargetHooks();

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

