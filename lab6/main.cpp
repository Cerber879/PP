#include <iostream>
#include <omp.h>

const int N = 100'000'000;

// Функция для синхронного вычисления PI
void CalculatePISinc() 
{
    double pi = 0.0;
    double startTime, endTime;

    startTime = omp_get_wtime();

    for (int i = 0; i < N; ++i)
    {
        double term = (i % 2 == 0) ? 1.0 / (2 * i + 1) : -1.0 / (2 * i + 1);
        pi += term;
    }

    pi *= 4.0;
    endTime = omp_get_wtime();

    std::cout << "PI (Sync): " << pi << std::endl;
    std::cout << "Time (Sync): " << endTime - startTime << " seconds" << std::endl;
    std::cout << std::endl;
}

// Функция для параллельного вычисления PI без синхронизации
void CalculatePIParallelNoSync()
{
    double pi = 0.0;
    double startTime, endTime;

    startTime = omp_get_wtime();

#pragma omp parallel for
    for (int i = 0; i < N; ++i) 
    {
        double term = (i % 2 == 0) ? 1.0 / (2 * i + 1) : -1.0 / (2 * i + 1);
        pi += term;
    }

    pi *= 4.0;
    endTime = omp_get_wtime();

    std::cout << "PI (Parallel No Sync): " << pi << std::endl;
    std::cout << "Time (Parallel No Sync): " << endTime - startTime << " seconds" << std::endl;
    std::cout << std::endl;
}

// Функция для параллельного вычисления PI с использованием atomic
void CalculatePIParallelAtomic()
{
    double pi = 0.0;
    double startTime, endTime;

    startTime = omp_get_wtime();

#pragma omp parallel for
    for (int i = 0; i < N; ++i) 
    {
        double term = (i % 2 == 0) ? 1.0 / (2 * i + 1) : -1.0 / (2 * i + 1);
#pragma omp atomic
        pi += term;
    }

    pi *= 4.0;
    endTime = omp_get_wtime();

    std::cout << "PI (Parallel Atomic): " << pi << std::endl;
    std::cout << "Time (Parallel Atomic): " << endTime - startTime << " seconds" << std::endl;
    std::cout << std::endl;
}

// Функция для параллельного вычисления PI с использованием reduction
void CalculatePIParallelReduction()
{
    double pi = 0.0;
    double startTime, endTime;

    startTime = omp_get_wtime();

#pragma omp parallel for reduction(+:pi)
    for (int i = 0; i < N; ++i)
    {
        double term = (i % 2 == 0) ? 1.0 / (2 * i + 1) : -1.0 / (2 * i + 1);
        pi += term;
    }

    pi *= 4.0;
    endTime = omp_get_wtime();

    std::cout << "PI (Parallel Reduction): " << pi << std::endl;
    std::cout << "Time (Parallel Reduction): " << endTime - startTime << " seconds" << std::endl;
    std::cout << std::endl;
}

int main() 
{
    CalculatePISinc();
    CalculatePIParallelNoSync();
    CalculatePIParallelAtomic();
    CalculatePIParallelReduction();

    return 0;
}
