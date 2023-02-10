#include "pch.h"
#include "mem.h"

void mem::Patch(BYTE* dst, BYTE* src, unsigned int size)
{
	DWORD oldprotect; //To be dertemined
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect); //
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

void mem::Nop(BYTE* dst, unsigned int size)
{
	DWORD oldprotect; //To be dertemined
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect); //
	memset(dst, 0x90, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}


uintptr_t mem::FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets) //Vi bruger vector her så den dynamisk altid kan finde ud af mængden af offsets.
{
	uintptr_t addr = ptr; //Tager værdien af ptr variablet og bevare det i addr variablet.
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		addr = *(uintptr_t*)addr;
		addr += offsets[i]; //Tilføjer offsets indtil den har den fulde addresse.
	}
	return addr; //Returnere addressen.
}