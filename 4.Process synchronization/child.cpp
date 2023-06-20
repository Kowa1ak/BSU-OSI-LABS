#include <windows.h>
#include <iostream>
#include <string>
#pragma comment(lib, "kernel32.lib")

using namespace std;

int main(int argc, char *argv[])
{
    HANDLE hMutex;
    HANDLE hEvent;
    HANDLE hEndedEvent;
    cout << "Child " << atoi(argv[1]) << endl;
    hMutex = OpenMutexW(SYNCHRONIZE, FALSE, L"Mutex");
    if (hMutex == NULL)
    {
        DWORD dwLastError = GetLastError();
        cout << "OpenMutex failed with error " <<  dwLastError << endl; 
        cout << "Press any key to exit." << endl;
        cin.get();
        return dwLastError;
    }

    hEvent = OpenEventW(EVENT_MODIFY_STATE, FALSE, L"ChildEvent " + atoi(argv[1]));
    if (hEvent == nullptr)
    {
        cout << "Open event failed." << endl;
        cout << "Input any char to exit." << endl;
        cin.get();
        return GetLastError();
    }

    hEndedEvent = OpenEventW(EVENT_MODIFY_STATE, FALSE, L"ChildEndedEvent " + atoi(argv[1]));
    if (hEvent == nullptr)
    {
        cout << "Open ending event failed." << endl;
        cout << "Input any char to exit." << endl;
        cin.get();
        return GetLastError();
    }

    string buffer;
    for (int i = 0, msg, nMsg = atoi(argv[0]); i < nMsg; i++)
    {
        WaitForSingleObject(hMutex, INFINITE);
        cout << "Event " << i + 1 << endl;
        cin >> buffer;
        SetEvent(hEvent);
        ReleaseMutex(hMutex);
    }

    SetEvent(hEndedEvent);

    CloseHandle(hMutex);
    CloseHandle(hEvent);
    CloseHandle(hEndedEvent);
    return 0;
}