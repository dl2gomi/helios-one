#pragma once
// memory.h
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>

class Memory {
public:
    DWORD GetProcessId(const std::wstring& processName);
    uintptr_t GetModuleBaseAddress(DWORD processId, const std::wstring& moduleName);
    template <typename T> T ReadMemory(HANDLE process, uintptr_t address);
    template <typename T> void WriteMemory(HANDLE process, uintptr_t address, T value);
};


