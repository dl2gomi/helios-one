// glowesp.h
#pragma once
#include <Windows.h>
#include <iostream>
#include "memory.h"

struct GlowObject {
    float r;
    float g;
    float b;
    float a;
    uint8_t m_bRenderWhenOccluded;
    uint8_t m_bRenderWhenUnoccluded;
    uint8_t m_bFullBloom;
    uint8_t pad;
    int32_t m_nGlowStyle;
};

class GlowESP {
public:
    GlowESP(Memory& memory, HANDLE processHandle, uintptr_t clientBase);
    void Run();

private:
    Memory& memory_;
    HANDLE processHandle_;
    uintptr_t clientBase_;
};
