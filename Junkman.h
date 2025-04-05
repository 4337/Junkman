#pragma once

#define J_DEBUG
#include <cstdio>

namespace J {

	/// <summary>
	/// Bycie po stronie "good guys" ma tê zalete ¿e mo¿emy wszystko pisaæ jako POC.
	/// Wykorzystywanie niskopoziomowego kodu który ju¿ istnieje ma tê wadê ¿e kod w plikach wykonywalnych 
	/// jest doœæ "schematyczny" np. mamy prologi i eplilogi zazwyczaj w 32-bitowym kodzie, FPO, które te¿ jest jednak doœæ oczywiste - ogólnie kod zwi¹zany z sekwencj¹ wywo³añ.
	/// W x64 mamy shadow-stack, czy wkoñcu jakieœ wype³niacze typu INT 3 w przypadku procesów które s¹ debugowane, lub œmieci miêdzy jedn¹ procedur¹ a nastêpno¹.
	/// Kiedy chcemy kopiowaæ istniej¹cy kod w niecnych celach to pewnie chcieli byœmy omin¹æ te "oczywiste" sekwencje bo mog³oby siê okazaæ ¿e nasz "junk code" to wielu przypadkach 
	/// prolog funkcji i z tego powodu mamy w³aœnie poni¿sze wyliczenie, które jest parametrem metody oznaczonym jako "RESERVED".
	/// </summary>
	
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