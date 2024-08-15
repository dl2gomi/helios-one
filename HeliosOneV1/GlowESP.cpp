// glowesp.cpp
#include "glowesp.h"

struct GlowObjectDefinition_t {
    float red;
    float green;
    float blue;
    float alpha;
    uint8_t renderWhenOccluded;
    uint8_t renderWhenUnoccluded;
    uint8_t fullBloom;
    uint8_t pad[5];
    int32_t glowStyle;
};

// Function to apply GlowESP to a player
void ApplyGlowESP(uintptr_t entityBase, int glowIndex, bool isEnemy) {
    // Read the address of the glow object manager (replace with actual offset for glow object manager)
    uintptr_t glowObjectManager = memory_.ReadMemory<uintptr_t>(processHandle_, clientBase_ + 0xXXXXXX);  // Replace with actual offset for glow object manager

    // Calculate the glow object address based on the glow index
    uintptr_t glowObject = glowObjectManager + (glowIndex * sizeof(GlowObjectDefinition_t));

    // Define the color (red for enemies, green for teammates)
    GlowObjectDefinition_t glow;
    if (isEnemy) {
        glow.red = 1.0f;  // Red
        glow.green = 0.0f;  // No green
        glow.blue = 0.0f;  // No blue
    }
    else {
        glow.red = 0.0f;  // No red
        glow.green = 1.0f;  // Green
        glow.blue = 0.0f;  // No blue
    }
    glow.alpha = 0.8f;  // Transparency
    glow.renderWhenOccluded = true;  // Render even if behind walls
    glow.renderWhenUnoccluded = false;
    glow.fullBloom = false;  // No full bloom effect
    glow.glowStyle = 0;  // Default glow style

    // Write the glow settings back to the memory
    memory_.WriteMemory<GlowObjectDefinition_t>(processHandle_, glowObject, glow);
}

// Function to apply GlowESP to all players
void GlowESP(uintptr_t localPlayer, uintptr_t clientBase) {
    // Loop through all player entities (up to 64 players in the game)
    for (int i = 0; i < 64; ++i) {
        // Calculate the address of the player entity (replace with correct offset for entity list)
        uintptr_t entityBase = memory_.ReadMemory<uintptr_t>(processHandle_, clientBase + 0xXXXXXX + i * 0x10);  // Replace with actual entity list offset

        // Check if the entity is valid
        if (entityBase) {
            // Read the entity's team (replace with actual offset for team)
            int entityTeam = memory_.ReadMemory<int>(processHandle_, entityBase + 0xXXXXXX);  // Replace with correct offset for team
            int localTeam = memory_.ReadMemory<int>(processHandle_, localPlayer + 0xXXXXXX);  // Replace with actual offset for local player's team

            // Read the entity's glow index (replace with actual offset for glow index)
            int glowIndex = memory_.ReadMemory<int>(processHandle_, entityBase + 0xXXXXXX);  // Replace with actual offset for glow index

            // Apply the glow (red for enemies, green for teammates)
            ApplyGlowESP(entityBase, glowIndex, entityTeam != localTeam);
        }
    }
}