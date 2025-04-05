#include <Windows.h>
#include "Memory.h"


J::Memory::Memory() noexcept(true) : bytes_{ 0 } {

	handler_ = INVALID_HANDLE_VALUE;

}

J::Memory::Memory(DWORD pid) noexcept(true) : bytes_{ 0 } {

	handler_= OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);

}

J::Memory::Memory(const TCHAR* path) noexcept(true) : bytes_{ 0 } { //trzeba zrobic file mapping (mape pliku w pamieci) 

	handler_ = CreateFile(path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_SYSTEM, NULL);  

}


J::Memory::~Memory() {

    if (handler_ != INVALID_HANDLE_VALUE && handler_ != NULL) {
	    CloseHandle(handler_);
	    handler_ = NULL;
     }

	if (bytes_.raw_ != nullptr) {
		delete[] bytes_.raw_;
	}

}

bool 
J::Memory::Opened() const noexcept(true) {

	return  (handler_ != INVALID_HANDLE_VALUE && handler_ != NULL) ? true : false;

}