#pragma once

#include <string>

namespace J {

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