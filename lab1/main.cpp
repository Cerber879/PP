#include <windows.h>
#include <string>
#include <iostream>

DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{
	int threadNumber = *(int*)lpParam;
	printf("����� N� %d %s", threadNumber, " ��������� ���� ������\n");

	ExitThread(0); // ������� ������������� ��� ���������� ������ � 0
}

int main()
{
	setlocale(LC_ALL, "ru");

	// ������� ���������� ������� �������������
	int N;
	std::cout << "������� ���������� �������: ";
	std::cin >> N;

	// �������� �������
	HANDLE* handles = new HANDLE[N];
	int* nums = new int[N];

	for (int i = 0; i < N; i++)
	{
		nums[i] = i;
		handles[i] = CreateThread(NULL, 0, &ThreadProc, &nums[i], CREATE_SUSPENDED, NULL);
	}

	// ������ �������
	for (int i = 0; i < N; i++)
	{
		ResumeThread(handles[i]);
	}

	// �������� ��������� ������ �������
	WaitForMultipleObjects(N, handles, true, INFINITE);

	return 0;
}
	