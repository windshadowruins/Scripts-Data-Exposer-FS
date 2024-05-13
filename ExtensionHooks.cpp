#include "pch.h"
#include "Logger.h"
#include "world/ExtensionHooks.h"
#include "bullet/BulletPatch.h"
#include "game/AOBScanCore.h"
#include "game/ExtensionScanner.h"
#include "tae/rootMotionReduction/RootMotionReductionPatch.h"
#include "target/TargetNpcPatch.h"
#include "world/ExtensionMasks.h"
#include "world/WorldInfo.h"


extern intptr_t getProcessBase();

void initTargetHooks()
{
    // 48 8B 48 08 49 89 8D
    // for CE scanning
    Logger::info("About to scan for target structure...\n");
    void* targetStructureMoveInstruction = scan(PLAYER_TARGET);

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
    Logger::info("Pointer to Jump is %p \n", jumpAddress);
    Logger::info("Pointer to start of after-Jump instruction is %p \n", (void*)start_of_next_instruction);
    Logger::info("Pointer to Decorator is %p \n", (void*)masmRoutineAddress);


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
    targetNpcInfo->x = DEFAULT_COORDINATES;
    targetNpcInfo->y = DEFAULT_COORDINATES;
    targetNpcInfo->z = DEFAULT_COORDINATES;

    Logger::info("Target NPC info at : %p\n", targetNpcInfo);
}

void initCreateBulletHook()
{
    // F3 0F 11 B3 14 0B 00 00 F3 0F 11 B3 18 0B 00 00 0F 28 74 24 40 48 83 C4 50 5B C3
    // for CE scanning

    // 48 B9 30 4F 34 3C FF
    // For searching hook

    Logger::info("About to scan for Create Bullet...\n");
    void* createBulletInvariant = scan(CREATE_BULLET);
    intptr_t createBulletInvariantAddress = (intptr_t)createBulletInvariant;
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
    Logger::info("About to scan for character List...\n");
    void* worldChrManInvariant = scan(CHARACTER_LIST);
    intptr_t worldChrManInvariantAddress = (intptr_t)worldChrManInvariant;
    int worldChrManInvariantAddressOffset19 = *(int*)(worldChrManInvariantAddress + 19);
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
    Logger::info("About to scan for Root Motion Reduction access...\n");

    void* rmrAccessInvariant = scan(ROOT_MOTION_REDUCTION_FACTOR_ACCESS);
    intptr_t rmrAccessInvariantAddress = (intptr_t)rmrAccessInvariant;
    
    unsigned char asmCode[] = {
    0x49, 0x89, 0xE2, 0x48, 0xB9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xD1, 0x90, };
    unsigned char* jumpAddress = reinterpret_cast<unsigned char*>(rmrAccessInvariant);
    
    DWORD oldProtect;
    VirtualProtect(jumpAddress, sizeof(asmCode), PAGE_EXECUTE_READWRITE, &oldProtect);
    void* allocatedMemory = rmr_decorator;
    unsigned char* start_of_next_instruction = jumpAddress + SIZE_OF_CALL_INSTRUCTION;
    unsigned char* masmRoutineAddress = reinterpret_cast<unsigned char*>(allocatedMemory);
    Logger::info("Pointer to Jump from is %p \n", jumpAddress);
    Logger::info("Pointer to Decorator is %p \n", (void*)masmRoutineAddress);
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
    Logger::info("Temp Factor pointer is at %p \n", (void*)taeEditor->rootMotionReductionFactor);
}
