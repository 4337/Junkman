#pragma once

namespace J {

	class File : public Memory {

	public:
		File(const TCHAR* path) noexcept(true);

		virtual code_block Code(DWORD ins_off, DWORD ins_cnt, int skip = J::NONE);

	};
};