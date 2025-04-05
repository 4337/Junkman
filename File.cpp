#include <Windows.h>

#include "Junkman.h"
#include "Memory.h"
#include "File.h"
#include "Disasm/hde64.h"


J::File::File(const TCHAR* path) noexcept(true) : Memory(path) {


}

J::code_block J::File::Code(DWORD ins_off, DWORD ins_cnt, int skip) {

    /*
    homework
    */
    code_block ret;
    ret.size_ = 0;
    ret.raw_ = (unsigned char*)"blabla";
    return ret;

}