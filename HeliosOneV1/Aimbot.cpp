#include <Windows.h>
#include <cmath> // For atan2, hypot, fabs
#include <iostream> // For std::cout

#define M_PI 3.14159265358979323846

struct Vector3 {
    float x, y, z;
};

struct ViewAngles {
    float pitch;
    float yaw;
};

// Function to calculate the angle to the target
Vector3 CalculateAngle(const Vector3& localPosition, const Vector3& enemyPosition) {
    Vector3 delta;
    delta.x = enemyPosition.x - localPosition.x;
    delta.y = enemyPosition.y - localPosition.y;
    delta.z = enemyPosition.z - localPosition.z;

    // Calculate yaw (left-right rotation)
    float yaw = atan2(delta.y, delta.x) * (180.0f / M_PI);

    // Calculate pitch (up-down rotation)
    float distance = hypot(delta.x, delta.y); // Hypotenuse of x and y (distance on 2D plane)
    float pitch = atan2(-delta.z, distance) * (180.0f / M_PI);

    // Return the angles as a vector (pitch and yaw)
    return { pitch, yaw, 0.0f };
}

// Calculate the absolute angular difference between two angles
float GetAngleDifference(float angle1, float angle2) {
    float diff = fmodf(angle1 - angle2 + 180.0f, 360.0f) - 180.0f;
    return fabs(diff);
}

// Check if the enemy is within the field of view
bool IsWithinFOV(const ViewAngles& currentAngles, const ViewAngles& targetAngles, float fov) {
    float pitchDifference = GetAngleDifference(targetAngles.pitch, currentAngles.pitch);
    float yawDifference = GetAngleDifference(targetAngles.yaw, currentAngles.yaw);

    return pitchDifference <= fov && yawDifference <= fov;
}

// Smoothing function
ViewAngles SmoothAim(const ViewAngles& currentAngles, const ViewAngles& targetAngles, float smoothFactor) {
    ViewAngles smoothedAngles;
    smoothedAngles.pitch = currentAngles.pitch + (targetAngles.pitch - currentAngles.pitch) / smoothFactor;
    smoothedAngles.yaw = currentAngles.yaw + (targetAngles.yaw - currentAngles.yaw) / smoothFactor;
    return smoothedAngles;
}

// Function to read bone position from bone matrix
Vector3 GetBonePosition(uintptr_t entityBase, int boneId) {
    // Read the base address of the bone matrix (replace with actual offset for bone matrix)
    uintptr_t boneMatrixBase = memory_.ReadMemory<uintptr_t>(processHandle_, entityBase + 0xXXXXXX); // Replace with actual offset

    // Each bone's position is stored in the bone matrix in the form of three floats (X, Y, Z).
    Vector3 bonePosition;
    bonePosition.x = memory_.ReadMemory<float>(processHandle_, boneMatrixBase + 0x30 * boneId + 0x0C); // X coordinate
    bonePosition.y = memory_.ReadMemory<float>(processHandle_, boneMatrixBase + 0x30 * boneId + 0x1C); // Y coordinate
    bonePosition.z = memory_.ReadMemory<float>(processHandle_, boneMatrixBase + 0x30 * boneId + 0x2C); // Z coordinate

    return bonePosition;
}

// Check if an enemy is visible (example, depends on the game)
bool IsEnemyVisible(uintptr_t entityBase) {
    // Read visibility flag from the enemy entity's data (replace with actual offset)
    bool visible = memory_.ReadMemory<bool>(processHandle_, entityBase + 0xXXXXXX); // Replace with correct offset
    return visible;
}

// Aimbot with FOV, Smoothing, Visibility check, Mouse1 Press, and Bone Targeting
void Aimbot::AimAt(uintptr_t entityBase, float fov, float smoothFactor, int boneId) {
    // Check if Mouse1 is pressed
    if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000)) {
        // If Mouse1 is not pressed, return early and do nothing
        return;
    }

    // Read the local player's position and view angles
    Vector3 localPosition = memory_.ReadMemory<Vector3>(processHandle_, localPlayer + 0xXXXXXX); // Replace with actual offset
    ViewAngles currentViewAngles = memory_.ReadMemory<ViewAngles>(processHandle_, localPlayer + 0xXXXXXX); // Replace with actual offset

    // Get the bone position of the enemy
    Vector3 enemyBonePosition = GetBonePosition(entityBase, boneId);

    // Check if the enemy is visible
    if (!IsEnemyVisible(entityBase)) {
        std::cout << "Enemy is not visible. Skipping..." << std::endl;
        return;
    }

    // Calculate the angles needed to aim at the enemy's bone
    Vector3 aimAngles = CalculateAngle(localPosition, enemyBonePosition);

    // Convert to ViewAngles struct for comparison
    ViewAngles targetAngles = { aimAngles.x, aimAngles.y };

    // Check if the enemy is within the specified FOV (adjustable)
    if (IsWithinFOV(currentViewAngles, targetAngles, fov)) {
        // Smooth the transition between current view angles and the target angles
        ViewAngles smoothedAngles = SmoothAim(currentViewAngles, targetAngles, smoothFactor);

        // Write the smoothed angles to the player's view angles
        memory_.WriteMemory<ViewAngles>(processHandle_, localPlayer + 0xXXXXXX, smoothedAngles); // Replace with actual offset
        std::cout << "Aiming at enemy bone with smoothing!" << std::endl;
    }
    else {
        std::cout << "Target is outside FOV." << std::endl;
    }
}