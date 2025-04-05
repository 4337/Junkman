// Junkman.cpp : Definiuje funkcje biblioteki statycznej.
//

#include <Windows.h>
#include <stdio.h>

#include "Junkman.h"

J::CodeSection J::GetCodeSection(HANDLE h_mod, LPVOID addr) {

	SIZE_T b_num;
	IMAGE_DOS_HEADER dos = { 0 };
	CodeSection code = { 0 };

	if (ReadProcessMemory(h_mod, addr, &dos, sizeof(IMAGE_DOS_HEADER), &b_num)) {
		if (b_num == sizeof(IMAGE_DOS_HEADER)) {
			LONG64 offset = reinterpret_cast<LONG64>(addr) +dos.e_lfanew;
			IMAGE_NT_HEADERS64 nt = { 0 };
			if (ReadProcessMemory(h_mod, reinterpret_cast<LPCVOID>(offset), &nt, sizeof(IMAGE_NT_HEADERS64), &b_num)) {
				offset = reinterpret_cast<LONG64>(addr);
				offset += nt.OptionalHeader.BaseOfCode;
				code.size_ = nt.OptionalHeader.SizeOfCode;
				code.base_addr_ = (FARPROC)offset;
			}
		}
	}
	return code;
}