#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <mmsystem.h>
#include <cmath>

void Compute()
{
    double result = 0;
    for (int j = 0; j < 1'000'000; j++)
    {
        result += std::sin(j) + std::cos(j);
    }
}

struct ThreadData
{
    int threadNum;
    DWORD startTime;
};

DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{
    ThreadData* data = (ThreadData*)lpParam;
    int threadNum = data->threadNum;
    DWORD startTime = data->startTime;
    int numOperations = 20; 
    std::ofstream* outputFile = new std::ofstream("thread_times.txt", std::ios::app);

    for (int i = 0; i < numOperations; ++i)
    {
        Compute();
        DWORD currentTime = timeGetTime();
        DWORD elapsedTime = currentTime - startTime;

        //*outputFile << elapsedTime << std::endl;
        *outputFile << "Thread " << threadNum << ", Operation " << i + 1 << ": " << elapsedTime << " ms" << std::endl;
    }

    outputFile->close();
    delete outputFile;

    ExitThread(0);
}

int main()
{
    SetProcessAffinityMask(GetCurrentProcess(), 7);
    setlocale(LC_ALL, "RU");
    int N = 3;
    HANDLE* handles = new HANDLE[N];
    ThreadData* threadData = new ThreadData[N];
    DWORD startTime = timeGetTime(); // Запоминаем начальное время

    for (int i = 0; i < N; i++)
    {
        threadData[i].threadNum = i + 1;
        threadData[i].startTime = startTime;
        handles[i] = CreateThread(NULL, 0, &ThreadProc, &threadData[i], CREATE_SUSPENDED, NULL);
    }
    // ABOVE_NORMAL BELOW_NORMAL
    SetThreadPriority(handles[0], THREAD_PRIORITY_ABOVE_NORMAL);
    SetThreadPriority(handles[1], THREAD_PRIORITY_NORMAL);
    SetThreadPriority(handles[2], THREAD_PRIORITY_BELOW_NORMAL);


    for (int i = 0; i < N; i++)
    {
        ResumeThread(handles[i]);
    }

    WaitForMultipleObjects(N, handles, true, INFINITE);

    delete[] handles;
    delete[] threadData;

    return 0;
}
