#include <iostream>
#include <windows.h>
#include <vector>

using namespace std;

CRITICAL_SECTION cs;
HANDLE event;
vector<char> arr;
char X;
int count_k = 0;

DWORD WINAPI work(LPVOID lpParam) {

	EnterCriticalSection(&cs);
	cout << "th w\n";
	int sleepTime;
	cout << "Enter sleep time: ";
	cin >> sleepTime;

	int left = 0;
	for (int i = 0; i < arr.size(); i++) {
		if (arr[i] == X) {
			swap(arr[left], arr[i]);
			left++;
		}
	}
	

	for (int i = 0; i < arr.size(); i++) {
		cout << arr[i] << " ";
		Sleep(sleepTime);
	}
	cout << endl;
	LeaveCriticalSection(&cs);
	cout << "end en w";

	return 0;
}

DWORD WINAPI CountElement(LPVOID lpParam) {
	

	EnterCriticalSection(&cs);
	cout << "поток count_k";
	
	for (int i = 0; i < arr.size(); i++) {
		if (arr[i] == X) count_k++;
	}

	LeaveCriticalSection(&cs);

	cout << " end поток count_k";
	SetEvent(event);
	return 0;
}

int main() {
	InitializeCriticalSection(&cs);
	event = CreateEvent(NULL, FALSE, FALSE, NULL);

	int n;
	cout << "Enter array size: ";
	cin >> n;

	arr.resize(n);
	cout << "Enter array elements: ";
	for (int i = 0; i < n; i++) cin >> arr[i];

	cout << "Array size: " << n << endl;
	cout << "Array elements: ";
	for (int i = 0; i < n; i++) cout << arr[i] << " ";
	cout << endl;

	HANDLE hThread1 = CreateThread(NULL, 0, work, NULL, CREATE_SUSPENDED, NULL);
	HANDLE hThread2 = CreateThread(NULL, 0, CountElement, NULL, CREATE_SUSPENDED, NULL);

	cout << "Enter symbol X:";
	cin >> X;

	ResumeThread(hThread1);
	ResumeThread(hThread2);
	WaitForSingleObject(event, INFINITE);
	cout << "Number of elements equal to " << X << ": " << count_k << endl;
	WaitForSingleObject(hThread1, INFINITE);
	WaitForSingleObject(hThread2, INFINITE);


	CloseHandle(hThread1);
	CloseHandle(hThread2);

	DeleteCriticalSection(&cs);
	CloseHandle(event);

	return 0;
}

