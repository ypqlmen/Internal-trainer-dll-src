#include "pch.h"
#include "proc.h"

DWORD GetProcId(const wchar_t* procName) //Creating the function to get process ID of game.
{
	DWORD procId = 0; //Vi sætter den til 0 for at gøre det nemmere at finde fejl.
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); //CreateToolhelp32Snapshot tager et snapshot, TH32CS_SNAPPROCESS(alle processer) er det vi vil have et snapshot af.
	if (hSnap != INVALID_HANDLE_VALUE) //Vi laver if statement så programmet kan fortælle os om der en fejl.
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hSnap, &procEntry)) //Process32First tager den første process i snapshottet og bevare den i procEntry.
		{
			do
			{
				if (!_wcsicmp(procEntry.szExeFile, procName)) //If statement that ask if procEntry is an exe of the same name as procName variable.
				{
					procId = procEntry.th32ProcessID; // Assigning process ID to procId
					break;
				}
							
			} while (Process32Next(hSnap, &procEntry)); //Leder efter vores valgte process.
		}
	}
	CloseHandle(hSnap); //Lukker handle for at stoppe memory leaks.
	return procId; //Gemmer process ID.
}

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName) //uintptr_t er en data type der kan virke lige meget hvilken bit det er(x86, x64)
{
	uintptr_t modBaseAddr = 0; //Vi bevare 0 i variablet for at gøre det lettere at teste for fejl.
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId); //Tager snapshot af modulet vi bruger ud fra process ID.
	if (hSnap != INVALID_HANDLE_VALUE) //Fejl kode til hvis snapshottet fejler.
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry)) //Tager det første modul i snapshottet og bevare den i modEntry.
		{
			do
			{
				if (!_wcsicmp(modEntry.szModule, modName))
				{
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry)); //Leder efter vores valgte modul.
		}
	}
	CloseHandle(hSnap); //Lukker handle fr at stoppe memory leaks.
	return modBaseAddr; // Returnere modul addressen.
}

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets) //Vi bruger vector her så den dynamisk altid kan finde ud af mængden af offsets.
{
	uintptr_t addr = ptr; //Tager værdien af ptr variablet og bevare det i addr variablet.
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0); //Læser hvad der  i addr variablet og bevare det i addr variablet.
		addr += offsets[i]; //Tilføjer offsets indtil den har den fulde addresse.
	}
	return addr; //Returnere addressen.
}


