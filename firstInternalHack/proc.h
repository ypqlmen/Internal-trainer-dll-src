#pragma once
#include <vector>
#include <windows.h>
#include <tlhelp32.h>

DWORD GetProcId(const wchar_t* procName); //Delcaration of function to get process id of game.

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName); //Delcaration of function to get module address of game.

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets); //Delcaration of function to add offsets to base address to get true address of memory.

