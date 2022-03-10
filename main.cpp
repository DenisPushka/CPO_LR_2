#include <Windows.h>
#include <tlhelp32.h>
#include <clocale>
#include <iostream>
#include <tchar.h>
#include <psapi.h>
#include <pdh.h>

#include <string>
#include <xinput.h>

#pragma comment(lib, "pdh.lib")
using namespace std;

void moduleEntry(int p);

void printProcess();

void printOneProcess(int p);

void timeProcess(int p);

int _tmain(int argc, _TCHAR *argv[]) {
    system("chcp 65001");

    printProcess();
    int process;
    cout << "\nВведите процесс: ";
    cin >> process;
    printOneProcess(process);
    timeProcess(process);
    moduleEntry(process);

    HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPALL, process);
    PROCESS_MEMORY_COUNTERS memCounter;
    if (GetProcessMemoryInfo(hProcess, &memCounter, sizeof(memCounter)))
        cout << "\n\n\tКол-во страничных ошибок: " << memCounter.PageFaultCount;
    else
        cout << "-";

    return 0;
}

void timeProcess(int process) {
    auto hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, process);

    PROCESSENTRY32 P;
    P.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hSnap, &P)) {
        do {
            if (P.th32ProcessID == process) {
                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, P.th32ProcessID);
                FILETIME ft[4];
                GetProcessTimes(hProcess, &ft[0], &ft[1], &ft[2], &ft[3]);
                SYSTEMTIME smPR, smUs;
                FileTimeToSystemTime(&ft[2], &smPR);
                FileTimeToSystemTime(&ft[3], &smUs);
                cout << "\n\tВремя в режиме ядра: " << smPR.wMilliseconds << "\tВ режиме пользователя: "
                     << smUs.wMilliseconds;
                break;
            }
        } while (Process32Next(hSnap, &P));
        CloseHandle(hSnap);
    }
}

void moduleEntry(int process) {
    auto hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process);
    MODULEENTRY32 Pe;
    Pe.dwSize = sizeof(MODULEENTRY32);
    if (Module32First(hSnap, &Pe)) {
        do {
            if (Pe.th32ProcessID == process) {
                cout << "\n\tИмя модуля: " << Pe.szModule << "\tРазмер: " << Pe.dwSize << endl;
                break;
            }
        } while (Module32Next(hSnap, &Pe));
        CloseHandle(hSnap);
    }
}

void printOneProcess(int process) {
    auto hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, process);
    PROCESSENTRY32 Pe;
    Pe.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hSnap, &Pe)) {
        do {
            if (process == Pe.th32ProcessID) {
                cout << "\n\tПроцесс: " << Pe.th32ProcessID << " \tБазовый приоритет процессора: " << Pe.pcPriClassBase
                     << "\tКол-во потоков: " << Pe.cntThreads << endl << endl;
                break;
            }
        } while (Process32Next(hSnap, &Pe));
        CloseHandle(hSnap);
    }
}

void printProcess() {
    auto hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 Pe;
    Pe.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hSnap, &Pe)) {
        do {
            printf_s("%6d, %e", Pe.th32ProcessID, Pe.pcPriClassBase);
            wprintf(L"%1s %-30s \n", " ", Pe.szExeFile);
        } while (Process32Next(hSnap, &Pe));
        CloseHandle(hSnap);
    }
}