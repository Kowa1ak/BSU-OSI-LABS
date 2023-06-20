#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#pragma comment(lib, "kernel32.lib")

using namespace std;
int main()
{
    HANDLE hMutex = NULL;
    hMutex = CreateMutexW(NULL, FALSE, L"Mutex");
    if (hMutex == NULL)
    {
        cout << "Create mutex failed." << endl;
        cout << "Press any key to exit." << endl;
        cin.get();
        return GetLastError();
    }

    HANDLE hSemaphore = NULL;
    hSemaphore = CreateSemaphoreW(NULL, 2, 2, L"Semaphore");
    if (hSemaphore == NULL)
    {
        cout << "Create semaphore failed." << endl;
        cout << "Press any key to exit." << endl;
        cin.get();
        return GetLastError();
    }

    int nParentPr;
    int nChildPr;
    int nParentMsg;
    int nChildMsg;
    int nSumMsg = 0;
    cout << "Enter the number of Parent processes: ";
    cin >> nParentPr;
    cout << "Enter the number of Child processes: ";
    cin >> nChildPr;

    int nSumPr = nParentPr + nChildPr;
    HANDLE* hEvent = new HANDLE[2 * nSumPr];

    vector<STARTUPINFOW*> siParent(nParentPr);
    vector<STARTUPINFOW*> siChild(nChildPr);
    vector<PROCESS_INFORMATION*> piParent(nParentPr);
    vector<PROCESS_INFORMATION*> piChild(nChildPr);
    wstring temp;
    wchar_t* lpszCommandLine;
    for (int i = 0; i < nParentPr; i++)
    {
        hEvent[i] = CreateEventW(NULL, FALSE, FALSE, L"ParentEvent " + (i + 1));
        if (hEvent[i] == NULL)
        {
            return GetLastError();
        }
        hEvent[i + nSumPr] = CreateEventW(NULL, FALSE, FALSE, L"ParentEndedEvent " + (i + 1));
        if (hEvent[i + nSumPr] == NULL)
        {
            return GetLastError();
        }
        siParent[i] = new STARTUPINFOW;
        ZeroMemory(siParent[i], sizeof(STARTUPINFOW));
        siParent[i]->cb = sizeof(STARTUPINFOW);
        piParent[i] = new PROCESS_INFORMATION;
        cout << "Enter the number of messages from Parent " << i + 1 << ": ";
        cin >> nParentMsg;
        nSumMsg += nParentMsg;
        temp = to_wstring(nParentMsg) + L" " + to_wstring(i + 1);
        lpszCommandLine = new wchar_t[temp.length()];
        wcscpy_s(lpszCommandLine, temp.length() + 1, temp.c_str());
        if (!CreateProcessW(L"parent.exe", lpszCommandLine, NULL, NULL, FALSE,
            CREATE_NEW_CONSOLE, nullptr, NULL, siParent[i], piParent[i]))
        {
            cout << "The new process is not created." << endl;
            cout << "Press any key to exit." << endl;
            cin.get();
            CloseHandle(hMutex);
            return GetLastError();
        }
    }
    for (int i = 0; i < nChildPr; i++)
    {
        hEvent[i + nParentPr] = CreateEventW(NULL, FALSE, FALSE, L"ChildEvent " + (i + 1));
        if (hEvent[i + nParentPr] == NULL)
        {
            return GetLastError();
        }
        hEvent[i + nParentPr + nSumPr] = CreateEventW(NULL, FALSE, FALSE, L"ChildEndedEvent " + (i + 1));
        if (hEvent[i + nParentPr + nSumPr] == NULL)
        {
            return GetLastError();
        }
        siChild[i] = new STARTUPINFOW;
        ZeroMemory(siChild[i], sizeof(STARTUPINFOW));
        siChild[i]->cb = sizeof(STARTUPINFOW);
        piChild[i] = new PROCESS_INFORMATION;
        cout << "Enter the number of messages from Child " << i + 1 << ": ";
        cin >> nChildMsg;
        nSumMsg += nChildMsg;
        temp = to_wstring(nChildMsg) + L" " + to_wstring(i + 1);
        lpszCommandLine = new wchar_t[temp.length()];
        wcscpy_s(lpszCommandLine, temp.length() + 1, temp.c_str());
        if (!CreateProcessW(L"child.exe", lpszCommandLine, NULL, NULL, FALSE,
            CREATE_NEW_CONSOLE, nullptr, NULL, siChild[i], piChild[i]))
        {
            cout << "The new process is not created." << endl;
            cout << "Press any key to exit." << endl;
            cin.get();
            return GetLastError();
        }
    }

    for (int i = 0; i < nSumMsg + nSumPr; i++)
    {
        int prEventIndex = WaitForMultipleObjects(2 * nSumPr, hEvent, FALSE, INFINITE) - WAIT_OBJECT_0;
        if (prEventIndex < nParentPr)
        {
            cout << "Message from Parent " << prEventIndex + 1;
        }
        else
        {
            if (prEventIndex < nSumPr)
            {
                cout << "Message from Child " << prEventIndex + 1 - nParentPr;
            }
            else
            {
                if (prEventIndex < nParentPr + nSumPr)
                {
                    cout << "Parent " << prEventIndex - nSumPr + 1 << " ended";
                }
                else
                {
                    cout << "Child " << prEventIndex - nParentPr - nSumPr + 1 << " ended";
                }
            }
        }
        cout << endl;
    }

    for (int i = 0; i < nParentPr; i++)
    {
        WaitForSingleObject(piParent[i]->hProcess, INFINITE);
        CloseHandle(piParent[i]->hThread);
        CloseHandle(piParent[i]->hProcess);
        CloseHandle(hEvent[i]);
        CloseHandle(hEvent[i + nSumPr]);
        delete siParent[i];
        delete piParent[i];
    }

    for (int i = 0; i < nChildPr; i++)
    {
        WaitForSingleObject(piChild[i]->hProcess, INFINITE);
        CloseHandle(piChild[i]->hThread);
        CloseHandle(piChild[i]->hProcess);
        CloseHandle(hEvent[i + nParentPr]);
        CloseHandle(hEvent[i + nParentPr + nSumPr]);
        delete siChild[i];
        delete piChild[i];
    }

    CloseHandle(hMutex);
    CloseHandle(hSemaphore);
    return 0;
}