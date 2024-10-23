#include <iostream>
#include <string>
#include <windows.h>
#include <fstream>
#include <chrono>
#include <cmath>

std::ofstream firstLogs("log.txt");
std::ofstream secondLogs("log1.txt");
std::chrono::high_resolution_clock::time_point startTime;

void Compute()
{
    double result = 0;
    for (int j = 0; j < 1'000'000; j++)
    {
        result += std::sin(j) + std::cos(j);
    }
}

DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{
    for (int i = 0; i < 20; i++)
    {
        Compute();
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();

        std::string dur = std::to_string(elapsedTime);

        (int(lpParam) == 2) ? (secondLogs << dur << std::endl)
            : (firstLogs << dur << std::endl);
    }

    ExitThread(0);
}

int main()
{
    int N = 2;
    // Устанавливаем сродство процессора на один процессор (например, процессор 0)
    DWORD_PTR processAffinityMask = 1; 
    SetProcessAffinityMask(GetCurrentProcess(), processAffinityMask);

    system("pause");

    startTime = std::chrono::high_resolution_clock::now();

    HANDLE* handles = new HANDLE[N];

    for (int i = 0; i < N; i++)
    {
        int ThreadCount = i + 1;
        handles[i] = CreateThread(NULL, 0, &ThreadProc, (LPVOID)ThreadCount, CREATE_SUSPENDED, NULL);

        // SetThreadPriority(handles[i], i);
    }

    for (int i = 0; i < N; i++)
    {
        ResumeThread(handles[i]);
    }

    WaitForMultipleObjects(N, handles, true, INFINITE);

    delete[] handles;

    return 0;
}
