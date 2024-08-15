// main.cpp
#include <iostream>
#include "memory.h"
#include "aimbot.h"
#include "triggerbot.h"
#include "glowesp.h"

int main() {
    // Initialize memory manager and other required components
    Memory memory;

    DWORD processId = memory.GetProcessId(L"cs2.exe"); // Replace with correct process name
    HANDLE processHandle = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, processId);

    uintptr_t clientBase = memory.GetModuleBaseAddress(processId, L"client.dll"); // Replace with correct module name
    uintptr_t localPlayerBase = memory.ReadMemory<uintptr_t>(processHandle, clientBase + 0xXXXXXX); // Replace with actual offset for local player

    // Create the aimbot instance
    Aimbot aimbot(memory, processHandle, clientBase, localPlayerBase);

    // Example: Use the aimbot on a specific enemy
    float fov = 10.0f;  // Field of View for the aimbot
    float smoothFactor = 5.0f;  // Smoothing factor to make the aiming more natural
    int boneId = 8;  // Bone ID for the head (example)
    uintptr_t enemyEntityBase = 0x12345678;  // Example enemy base address, replace with actual offset
    int triggerbotDelay = 100; //Delay before shooting 

    // Infinite loop for continuous aimbot checking
    while (true) {
        Triggerbot(localPlayerBase, clientBase, triggerbotDelay);

        aimbot.AimAt(enemyEntityBase, fov, smoothFactor, boneId);

        GlowESP(localPlayerBase, clientBase);
        Sleep(1);  // Sleep to reduce CPU usage
    }

    CloseHandle(processHandle);
    return 0;
}