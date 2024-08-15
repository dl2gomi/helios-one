// triggerbot.h
#pragma once
#include <Windows.h>
#include <iostream>
#include "memory.h"

class Triggerbot {
public:
    Triggerbot(Memory& memory, HANDLE processHandle, uintptr_t clientBase);
    void Run();

private:
    Memory& memory_;
    HANDLE processHandle_;
    uintptr_t clientBase_;
};