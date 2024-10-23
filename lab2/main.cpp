#include <windows.h>
#include <string>
#include <iostream>
#include <chrono>
#include <vector>
#include <cmath>
#include "Bitmap.h"

struct Squares
{
    Bitmap* in;
    uint32_t startWidth;
    uint32_t endWidth;
    uint32_t startHeight;
    uint32_t endHeight;
    int kernelSize;
};

std::vector<std::vector<double>> GenerateGaussianKernel(int size, double sigma)
{
    std::vector<std::vector<double>> kernel(size, std::vector<double>(size));
    double sum = 0.0;

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            int x = i - size / 2;
            int y = j - size / 2;
            kernel[i][j] = exp(-(x * x + y * y) / (2 * sigma * sigma));
            sum += kernel[i][j];
        }
    }

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            kernel[i][j] /= sum;
        }
    }

    return kernel;
}

void GaussianBlur(Bitmap* in, uint32_t startWidth, uint32_t endWidth, uint32_t startHeight, uint32_t endHeight, int kernelSize)
{
    for (int i = 0; i < 15; i++)
    {
        double sigma = 1.0;
        std::vector<std::vector<double>> kernel = GenerateGaussianKernel(kernelSize, sigma);

        for (uint32_t y = startHeight; y < endHeight; ++y)
        {
            for (uint32_t x = startWidth; x < endWidth; ++x)
            {
                double r = 0, g = 0, b = 0;
                double weightSum = 0;

                for (int ky = 0; ky < kernelSize; ++ky)
                {
                    for (int kx = 0; kx < kernelSize; ++kx)
                    {
                        int px = x + kx - kernelSize / 2;
                        int py = y + ky - kernelSize / 2;

                        if (px >= 0 && px < in->GetWidth() && py >= 0 && py < in->GetHeight())
                        {
                            rgb32* pixel = in->GetPixel(px, py);
                            double weight = kernel[ky][kx];
                            r += pixel->r * weight;
                            g += pixel->g * weight;
                            b += pixel->b * weight;
                            weightSum += weight;
                        }
                    }
                }

                rgb32 newPixel;
                newPixel.r = static_cast<uint8_t>(r / weightSum);
                newPixel.g = static_cast<uint8_t>(g / weightSum);
                newPixel.b = static_cast<uint8_t>(b / weightSum);
                newPixel.a = 255;

                in->SetPixel(&newPixel, x, y);
            }
        }
    }
}

DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{
    Squares* squares = (Squares*)lpParam;
    GaussianBlur(squares->in, squares->startWidth, squares->endWidth, squares->startHeight, squares->endHeight, squares->kernelSize);
    ExitThread(0);
}

void StartThreads(Bitmap* in, int numThreads, int numCores)
{
    int sideHeight = in->GetHeight() / numThreads;
    int remainingHeight = in->GetHeight() % numThreads;

    int sideWidth = in->GetWidth() / numThreads;
    int remainingWidth = in->GetWidth() % numThreads;

    std::vector<Squares> arrSquares;
    for (int i = 0; i < numThreads; i++)
    {
        for (int j = 0; j < numThreads; j++)
        {
            Squares square;
            square.in = in;
            square.startWidth = sideWidth * j;
            square.endWidth = sideWidth * (j + 1) + ((j == numThreads - 1) ? remainingWidth : 0);
            square.startHeight = sideHeight * i;
            square.endHeight = sideHeight * (i + 1) + ((i == numThreads - 1) ? remainingHeight : 0);
            square.kernelSize = numThreads;
            arrSquares.push_back(square);
        }
    }

    HANDLE* handles = new HANDLE[numThreads * numThreads];
    for (int i = 0; i < numThreads * numThreads; i++)
    {
        handles[i] = CreateThread(NULL, 0, &ThreadProc, &arrSquares[i], CREATE_SUSPENDED, NULL);
        SetThreadAffinityMask(handles[i], (1 << numCores) - 1);
    }

    for (int i = 0; i < numThreads * numThreads; i++)
    {
        ResumeThread(handles[i]);
    }

    WaitForMultipleObjects(numThreads * numThreads, handles, true, INFINITE);

    delete[] handles;
}

int main(int argc, char* argv[])
{
    if (argc != 5)
    {
        std::cerr << "Usage: " << argv[0] << " <input.bmp> <output.bmp> <numThreads> <numCores>" << std::endl;
        return 1;
    }

    const char* inputFile = argv[1];
    const char* outputFile = argv[2];
    int numThreads = std::stoi(argv[3]);
    int numCores = std::stoi(argv[4]);

    auto start = std::chrono::high_resolution_clock::now();

    Bitmap bmp(inputFile);
    StartThreads(&bmp, numThreads, numCores);
    bmp.Save(outputFile);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cerr << "Time: " << elapsed.count() << " ms" << std::endl;

    return 0;
}
