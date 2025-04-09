#pragma once

#include <string>

namespace J {

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

	struct Section {

		DWORD  size_;
		FARPROC base_addr_;

	};

	using binary_string = std::basic_string<unsigned char>;

	struct code_block {
		SIZE_T size_;
		binary_string block_; //disasmed block
		FARPROC base_addr_;
		unsigned char* raw_;  //all
	};

	class Memory {

	protected:

		HANDLE handler_;

		code_block bytes_;   

	public:

		explicit Memory() noexcept(true);
		explicit Memory(DWORD pid) noexcept(true);
		explicit Memory(const TCHAR* path) noexcept(true);
	

		virtual code_block Code(DWORD ins_off, DWORD ins_cnt, int skip = 0x6) = 0;  
		virtual bool Opened() const noexcept(true); 

		virtual ~Memory();

	};

};