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

void moduleExp1();

void printProcess();

void printOneProcess(int p);

void PROCESS_MEMORY();

double timeProcess(int process) {
    FILETIME createTime;
    FILETIME exitTime;
    FILETIME kernelTime;
    FILETIME userTime;
    auto hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, process);
    if (GetProcessTimes(hSnap,
                        &createTime, &exitTime, &kernelTime, &userTime) != -1) {
        SYSTEMTIME userSystemTime;
        if (FileTimeToSystemTime(&userTime, &userSystemTime) != -1)
            return (double) userSystemTime.wHour * 3600.0 +
                   (double) userSystemTime.wMinute * 60.0 +
                   (double) userSystemTime.wSecond +
                   (double) userSystemTime.wMilliseconds / 1000.0;
    }
}

int _tmain(int argc, _TCHAR *argv[]) {
    system("chcp 65001");
    printProcess();
    moduleExp1();
    /*printProcess();
    int process;
    cout << "\nВведите процесс: ";
    cin >> process;
    printOneProcess(process);
    cout << "Время работы в редиме ядра: " << timeProcess(process);
    auto hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, process);

    //PROCESS_MEMORY_COUNTERS Pr;
    PROCESS_MEMORY_COUNTERS *Prq;
    DWORD cbSize;
    if (GetProcessMemoryInfo(hSnap, Prq, cbSize))
        cout << "\n\nКол-во страничных ошибок: " << Prq->PageFaultCount;
    return 0;*/
}

void PROCESS_MEMORY(int a) {
    cout << "\nВведите прикладной процесс: ";
    auto hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, a);
    PROCESSENTRY32 Pe;
    Pe.dwSize = sizeof(Pe);
    if (Process32First(hSnap, &Pe)) {
        do {
            if (a == Pe.th32ProcessID) {
                cout << "Процесс: " << Pe.th32ProcessID << " \tВремя работы в режиме ядра: " << Pe.pcPriClassBase
                     << "\tВремя работы в режиме пользователя: " << Pe.cntThreads << endl << endl;
                break;
            }
        } while (Process32Next(hSnap, &Pe));
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
                cout << "Процесс: " << Pe.th32ProcessID << " \tБазовый приоритет процессора: " << Pe.pcPriClassBase
                     << "\t\tКол-во потоков: " << Pe.cntThreads << endl << endl;
                break;
            }
        } while (Process32Next(hSnap, &Pe));
        CloseHandle(hSnap);
    }

}

void moduleExp1() {
    int a;
    cout << "Enter number process: ";
    cin >> a;
    auto hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, a);
    PROCESSENTRY32 Pe;
    Pe.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hSnap, &Pe)) {
        do {
            if (Pe.th32ParentProcessID == a) {
                cout << Pe.th32ProcessID << " " <<  Pe.th32ParentProcessID << endl;
                wprintf(L"%1s %-30s \n", " ", Pe.szExeFile);
                // break;
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