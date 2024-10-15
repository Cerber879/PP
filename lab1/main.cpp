#include <windows.h>
#include <string>
#include <iostream>

DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{
	int threadNumber = *(int*)lpParam;
	printf("Поток N№ %d %s", threadNumber, " выполняет свою работу\n");

	ExitThread(0); // функция устанавливает код завершения потока в 0
}

int main()
{
	setlocale(LC_ALL, "ru");

	// Задание количества потоков пользователем
	int N;
	std::cout << "Введите количество потоков: ";
	std::cin >> N;

	// создание потоков
	HANDLE* handles = new HANDLE[N];
	int* nums = new int[N];

	for (int i = 0; i < N; i++)
	{
		nums[i] = i;
		handles[i] = CreateThread(NULL, 0, &ThreadProc, &nums[i], CREATE_SUSPENDED, NULL);
	}

	// запуск потоков
	for (int i = 0; i < N; i++)
	{
		ResumeThread(handles[i]);
	}

	// ожидание окончания работы потоков
	WaitForMultipleObjects(N, handles, true, INFINITE);

	return 0;
}
	