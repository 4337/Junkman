#pragma once

#define J_DEBUG

namespace J {

	constexpr int PROLOGUE = 0x1;
	constexpr int EPILOGUE = 0x2;
	constexpr int SHADOW_STACK = 0x3;
	constexpr int FPO = 0x4;
	constexpr int DEBUG = 0x5;
	constexpr int NONE = 0x6;

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
	SITE_T SkipIAT()
	*/

};