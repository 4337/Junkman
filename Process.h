#pragma once


namespace J{

class Process : public Memory {

	bool self_proc_;

	FARPROC GetBaseAddresByHandle(HANDLE h_mod) noexcept(true);

	DWORD GetDllHandleByProcessId(DWORD pid, const TCHAR* dll) noexcept(true);
	DWORD GetPidByName(const TCHAR* name) const noexcept(true);

public:

	explicit Process(DWORD pid) noexcept(true);
	explicit Process(const TCHAR* proc_name) noexcept(true);
	explicit Process(DWORD pid, const TCHAR* mod_name) noexcept(true);
	explicit Process(const TCHAR* lib, const CHAR* proc) noexcept(true);

	inline bool Opened() const noexcept(true) override {
		return  (self_proc_  || (handler_ != INVALID_HANDLE_VALUE && handler_ != NULL)) ? true : false;
	}
	virtual code_block Code(DWORD ins_off, DWORD ins_cnt, int skip = J::NONE);

};

};
