#include <Windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <shlwapi.h>
#include <iostream>
#include <tchar.h>

#include "Junkman.h"
#include "Memory.h"
#include "Process.h"
#include "Disasm/hde64.h"

/// <summary>
/// DUPAJASIA
/// </summary>
/// <param name="h_mod"></param>
/// <returns></returns>
FARPROC J::Process::GetBaseAddresByHandle(HANDLE h_mod) noexcept(true) {

	DWORD need;
	HMODULE mods[1024] = { 0 };
	if (!EnumProcessModules(h_mod, mods, sizeof(mods), &need)) {
			return NULL;
	}
	return (FARPROC)mods[0];

}


/// <summary>
/// Pobierz processId na podstawie nazwy process
/// </summary>
/// <param name="name">const TCHAR* - wskaŸnik do nazwy procesu</param>
/// <returns>
/// -1 = error
///  0 = process nie zosta³ znaleziony
///  OK = identyfikator process  
/// </returns>
DWORD 
J::Process::GetPidByName(const TCHAR* name) const noexcept(true) {

	register DWORD ret = -1;
	HANDLE s_shot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (s_shot != INVALID_HANDLE_VALUE) {

		PROCESSENTRY32 p_entry;
		p_entry.dwSize = sizeof(PROCESSENTRY32);
	
		BOOL next = Process32First(s_shot, &p_entry);
		while (next) {
			if (_tcscmp(name, p_entry.szExeFile) == 0) {
				ret = p_entry.th32ProcessID;
				break;
			}
			next = Process32Next(s_shot, &p_entry);
		}
        
		CloseHandle(s_shot);
		return 0;
	}

	return ret;

}

/// <summary>
/// Otwiera process na podstawie processId
/// </summary>
/// <param name="pid">identyfikator processu</param>
/// <returns>konstruktor</returns>
J::Process::Process(DWORD pid) noexcept(true) : Memory(pid), self_proc_(false) {

	bytes_.base_addr_ = GetBaseAddresByHandle(handler_);

}

/// <summary>
/// Otwiera process na podstawie nazwy
/// </summary>
/// <param name="proc_name">nazwa procesu</param>
/// <returns>konstruktor</returns>
J::Process::Process(const TCHAR* proc_name) noexcept(true) : Memory(), self_proc_(false) {
	DWORD pid = GetPidByName(proc_name);
	if (pid != -1) {
		handler_ = OpenProcess(PROCESS_VM_READ, FALSE, pid);
		bytes_.base_addr_ = GetBaseAddresByHandle(handler_);
	}
}

/// <summary>
/// Pobiera uchwyt do pliku dll w procesie okreœlonym przez identyfikator processu
/// </summary>
/// <param name="pid">indetyfikator procesu</param>
/// <param name="dll">nazwa biblioteki dll której uchwytu potrzebujemy</param>
/// <returns>
/// -1 = error
///  0 = nie znaleziono dll-ki
///  1 = sukces
/// </returns>
DWORD
J::Process::GetDllHandleByProcessId(DWORD pid, const TCHAR* dll) noexcept(true) {

	register DWORD ret = -1;
	HANDLE h_proc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
	if (h_proc == INVALID_HANDLE_VALUE) {
		return ret;
	}

	DWORD n_cb = 0;
	bool next = true;
	size_t alloc_size = 1024;
	do {
		HMODULE* dlls = new HMODULE[alloc_size];
		if (dlls != nullptr) {

			if (!EnumProcessModulesEx(h_proc, dlls, static_cast<DWORD>(alloc_size), &n_cb, LIST_MODULES_64BIT))
			{
				next = false;
			}
			else if (n_cb > alloc_size) {
				alloc_size = n_cb;
			}
			else {
				for (size_t i = 0; i < alloc_size; i++) {
					if (dlls[i] != NULL && dlls[i] != INVALID_HANDLE_VALUE) {
						TCHAR name[MAX_PATH + 2] = { 0 };
						GetModuleFileName(dlls[i], name, MAX_PATH);
						if (_tcsicmp(PathFindFileName(name), dll) == 0) {
							handler_ = dlls[i];
							ret = 1;
						}
					}
				}
				if (ret == -1) {
					ret = 0;
				}
			}
			delete[] dlls;
		}
		else {
			break;
		}
	} while (next && ret == -1);

	CloseHandle(h_proc);
	return ret;

}

/// <summary>
/// 
/// </summary>
/// <param name="pid"></param>
/// <param name="mod_name"></param>
/// <returns></returns>
J::Process::Process(DWORD pid, const TCHAR* mod_name) noexcept(true) : Memory(), self_proc_(false) {

	GetDllHandleByProcessId(pid, mod_name);
	bytes_.base_addr_ = GetBaseAddresByHandle(handler_);

}

unsigned char* J::Process::Read(HANDLE h_mod, LPVOID base_addr, SIZE_T size) noexcept(true) {

	unsigned char* buff = new unsigned char[size + 1];
	if (buff != nullptr) {

		SIZE_T r_size;
		if (!ReadProcessMemory(h_mod, base_addr, buff, size, &r_size) == TRUE) {
			delete[] buff;
			buff = nullptr;
		}

	}
	return buff;

}

J::Process::Process(const TCHAR* lib, const CHAR* proc) noexcept(true) : self_proc_(false), Memory() {

	HMODULE mod = LoadLibrary(lib);
	if (mod != NULL) {
		if ((bytes_.base_addr_ = GetProcAddress(mod, proc)) != NULL) {
			handler_ = GetCurrentProcess();
			self_proc_ = true;
        }
 	}
	
}

J::code_block
J::Process::Code(DWORD ins_off, DWORD ins_cnt, int skip) {

	if (!self_proc_) {
		J::CodeSection cs = J::GetCodeSection(handler_, bytes_.base_addr_);
		if (cs.size_ != 0 && cs.base_addr_ != NULL) {

			bytes_.size_ = cs.size_;
			bytes_.base_addr_ = cs.base_addr_;

		}
	}
	else {
		bytes_.size_ = static_cast<SIZE_T>(ins_cnt) * 15;
	}
    
	bytes_.raw_ = new unsigned char[bytes_.size_ + 2];
	if (bytes_.raw_ == nullptr) {
		return { 0 };
	}

	SIZE_T readed;
	if (!ReadProcessMemory(handler_, bytes_.base_addr_, bytes_.raw_, bytes_.size_, &readed)) {
		delete[] bytes_.raw_;
		bytes_.raw_ = nullptr;
		return { 0 };
	}

	hde64s hde;
	SIZE_T total_len = 0, cnt = 0, copied = 0;
	do {
		hde64_disasm(bytes_.raw_ + total_len, &hde);
		
		if (DetectAndSkip(*(bytes_.raw_ + total_len), hde.len, skip)) {
			total_len += hde.len;
			continue;
		}

		if (cnt >= ins_off) { 
			
			bytes_.block_.append(bytes_.raw_ + total_len, hde.len);
            copied++;

		}

		if (copied == ins_cnt) {
			break;
		}

		total_len += hde.len;
		cnt++;

	} while (total_len < bytes_.size_);

	return bytes_;

}