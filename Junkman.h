#pragma once

#define J_DEBUG

namespace J {

	constexpr int PROLOGUE = 0x1;
	constexpr int EPILOGUE = 0x2;
	constexpr int SHADOW_STACK = 0x3;
	constexpr int FPO = 0x4;
	constexpr int DEBUG = 0x5;
	constexpr int NONE = 0x6;

#pragma pack(push, 2)
	typedef struct _IMAGE_DOS_HEADER
	{
		WORD e_magic;
		WORD e_cblp;
		WORD e_cp;
		WORD e_crlc;
		WORD e_cparhdr;
		WORD e_minalloc;
		WORD e_maxalloc;
		WORD e_ss;
		WORD e_sp;
		WORD e_csum;
		WORD e_ip;
		WORD e_cs;
		WORD e_lfarlc;
		WORD e_ovno;
		WORD e_res[4];
		WORD e_oemid;
		WORD e_oeminfo;
		WORD e_res2[10];
		LONG e_lfanew;
	} IMAGE_DOS_HEADER, * PIMAGE_DOS_HEADER;
#pragma pack(pop)


	struct CodeSection {

		DWORD  size_;
		FARPROC base_addr_;

	};

	CodeSection GetCodeSection(HANDLE h_mod, LPVOID addr = NULL);

	
	inline bool DetectShadowStack(UINT8 op, UINT8 ins_len) {
		return (op == 0x48 && ins_len == 4) ? true : false;
	}
	inline bool DetectDebug(UINT8 op, UINT8 ins_len) {
		return (op == 0xCC && ins_len == 1) ? true : false;
	}

	inline bool DetectAndSkip(UINT8 op, UINT8 ins_len, int skip) {

		if (skip & J::DEBUG) {
			return DetectDebug(op, ins_len);
		}
		/* homework */
		if (skip & J::EPILOGUE) {

		}
		if (skip & J::FPO) {

		}
		if (skip & J::PROLOGUE) {

		}
		if (skip & J::SHADOW_STACK) {
			return DetectShadowStack(op, ins_len);
		}
		return false;

	}

	/*
	bool DetectPrologue()
	bool DetectEpilogue()
	bool DetectFPO()
	void FixRelocations() 
	*/

};