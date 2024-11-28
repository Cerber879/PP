#include <iostream>
#include <omp.h>

void CalculateWithPrivate()
{
    std::cout << "Calculate with private(x):" << std::endl;

    int x = 44;

#pragma omp parallel for private(x)
    for (int i = 0; i <= 10; i++)
    {
        x = i;
        printf("Thread number: %d x: %d\n", omp_get_thread_num(), x);
    }

    printf("x is %d\n", x);
    std::cout << std::endl;
}

void CalculateWithFirstprivate() 
{
    std::cout << "Calculate with firstprivate(x):" << std::endl;

    int x = 44;

#pragma omp parallel for firstprivate(x)
    for (int i = 0; i <= 10; i++) 
    {
        x = i;
        printf("Thread number: %d x: %d\n", omp_get_thread_num(), x);
    }

    printf("x is %d\n", x);
    std::cout << std::endl;
}

void CalculateWithLastprivate() 
{
    std::cout << "Calculate with lastprivate(x):" << std::endl;

    int x = 44;

#pragma omp parallel for lastprivate(x)
    for (int i = 0; i <= 10; i++) 
    {
        x = i;
        printf("Thread number: %d x: %d\n", omp_get_thread_num(), x);
    }

    printf("x is %d\n", x);
    std::cout << std::endl;
}

int main()
{
    omp_set_num_threads(4);

    CalculateWithPrivate();
    CalculateWithFirstprivate();
    CalculateWithLastprivate();

    return 0;
}
