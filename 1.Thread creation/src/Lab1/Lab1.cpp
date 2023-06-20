#include <windows.h>
#include <iostream>

using namespace std;


DWORD WINAPI worker(LPVOID array) {
	int count = 0;
	for (int i = 1; i < ((int*)array)[0] + 1; i++) {
		if (((int*)array)[i] % 5 == 0) {
			count++;
		}
	}
	cout <<"Result: " << count;
	return 0;
}

int main()
{
	int n;
	cout << "Enter size of array: ";
	cin >> n;
	int* array = new int[n + 1];
	array[0] = n;
	cout << "Enter elements: ";
	for (int i = 1; i < n + 1; i++) {
		int element;
		cin >> element;
		array[i] = element;
	}
	HANDLE hThread;
	DWORD IDThread;
	hThread = CreateThread(NULL, 0, worker, (void*)array, 0, &IDThread);
	if (hThread == NULL) {
		return GetLastError();
	}
	SuspendThread(hThread);
	Sleep(5000);
	ResumeThread(hThread);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	delete[] array;
	return 0;
}