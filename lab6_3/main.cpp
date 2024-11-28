#include <iostream>
#include <omp.h>
#include <vector>

void initializeMatrix(std::vector<std::vector<int>>& matrix, int N) 
{
    for (int i = 0; i < N; ++i) 
    {
        for (int j = 0; j < N; ++j) 
        {
            matrix[i][j] = rand() % 5;
        }
    }
}

void printMatrix(const std::vector<std::vector<int>>& matrix, int N) 
{
    for (int i = 0; i < N; ++i) 
    {
        for (int j = 0; j < N; ++j) 
        {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void parallelMatrixMultiplication(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, std::vector<std::vector<int>>& C, int N) 
{
#pragma omp parallel for
    for (int i = 0; i < N; ++i) 
    {
        for (int j = 0; j < N; ++j)
        {
            for (int k = 0; k < N; ++k)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() 
{
    const int N = 4; 
    std::vector<std::vector<int>> A(N, std::vector<int>(N));
    std::vector<std::vector<int>> B(N, std::vector<int>(N));
    std::vector<std::vector<int>> C(N, std::vector<int>(N, 0));

    initializeMatrix(A, N);
    initializeMatrix(B, N);

    std::cout << "Matrix A:" << std::endl;
    printMatrix(A, N);
    std::cout << "Matrix B:" << std::endl;
    printMatrix(B, N);

    parallelMatrixMultiplication(A, B, C, N);

    std::cout << "Resulting Matrix C:" << std::endl;
    printMatrix(C, N);

    return 0;
}
