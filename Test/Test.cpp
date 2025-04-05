// Test.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//
#include <Windows.h>
#include <tchar.h>
#include <iostream>

#include <cstdio>

#include "../Junkman/Junkman.h"
#include "../Junkman/Memory.h"
#include "../Junkman/File.h"
#include "../Junkman/Process.h"


int main()
{
    J::Memory* proc = new J::Process(10284);
    if (proc->Opened()) {
        std::cout << "Process(10284) opened !" << std::endl;
    }
    //J::Memory* file = new J::File(_T("stefan.exe")); 
    //if (file->Opened()) {
    //    std::cout << "File(stefan.exe) opened !" << std::endl;
    //}
    J::Memory* proc1 = new J::Process(_T("Kernel32.dll"),"ReadProcessMemory");  
    if (proc1->Opened()) {
        std::cout << "Process(Kernel32.dll,ReadProcessMemory) opened !" << std::endl;
    }
    
    //reszta konstruktorow

    J::code_block hi = proc1->Code(0, 8, J::SHADOW_STACK | J::DEBUG);

    printf("0xEB 0x%02x JMP over block\r\n", hi.block_.length());
    for (size_t i = 0; i < hi.block_.length(); i++) {
        printf("hi.block_[i] = 0x%x\r\n", hi.block_[i]);
    }

    return 0;

}

// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln
