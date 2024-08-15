// triggerbot.cpp
#include "triggerbot.h"
#include <Windows.h>
#include <iostream>

// Triggerbot function: Fires when crosshair is over an enemy and Mouse3 is pressed
void Triggerbot(uintptr_t localPlayer, uintptr_t clientBase, int delayBeforeShoot) {
    // Check if Mouse3 is pressed (middle mouse button by default)
    if (!(GetAsyncKeyState(VK_MBUTTON) & 0x8000)) {
        // If Mouse3 is not pressed, return early and do nothing
        return;
    }

    // Read the crosshair target ID (replace with actual offset for crosshair target)
    int crosshairTargetId = memory_.ReadMemory<int>(processHandle_, localPlayer + 0xXXXXXX);  // Replace with actual offset for crosshair target
    if (crosshairTargetId > 0 && crosshairTargetId <= 64) {  // If the crosshair is on an entity (player/bot)
        // Calculate the address of the entity in the player list (replace with actual offset for entity list)
        uintptr_t targetEntity = memory_.ReadMemory<uintptr_t>(processHandle_, clientBase + 0xXXXXXX + (crosshairTargetId - 1) * 0x10);  // Replace with correct offset

        // Check if the target entity is an enemy and is alive (replace with the actual offsets for health and team)
        int targetHealth = memory_.ReadMemory<int>(processHandle_, targetEntity + 0xXXXXXX);  // Replace with actual offset for health
        int targetTeam = memory_.ReadMemory<int>(processHandle_, targetEntity + 0xXXXXXX);  // Replace with actual offset for team
        int localTeam = memory_.ReadMemory<int>(processHandle_, localPlayer + 0xXXXXXX);  // Replace with actual offset for local player's team

        // Only shoot if the target is an enemy and is alive
        if (targetHealth > 0 && targetTeam != localTeam) {
            // Add delay before shooting
            Sleep(delayBeforeShoot);

            // Simulate mouse click (shoot)
            INPUT input = { 0 };
            input.type = INPUT_MOUSE;
            input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;  // Left mouse button down
            SendInput(1, &input, sizeof(INPUT));

            input.mi.dwFlags = MOUSEEVENTF_LEFTUP;  // Left mouse button up
            SendInput(1, &input, sizeof(INPUT));

            std::cout << "Shot fired!" << std::endl;
        }
    }
}