# Laboratory work No. 4

## Process synchronization

- - -

### Task:

Write programs for the Boss console process and Parent, Child console processes. To simulate the transmission of messages, enter special events (with automatic reset) that indicate "A", "B" and the end of the session for the Parent and Child processes. You can only receive a message from ONE ACTIVE Child process (use a mutex) and TWO ACTIVE Parent processes (use semaphore), the transmission of other messages from other processes should be temporarily blocked (in standby mode).

<br>

**Boss Process:**
1. Initialization of synchronization objects;
2. Asks the user for the number of Parent processes and the number of Child processes that he
should run;
3. Requests the number of messages sent by Parent and the number of messages sent by Child (the total number of sent and received messages must match);
4. starts the specified number of Parent, Child processes;
5. Accepts messages from each Parent, Child process, outputs messages and who sent it to the console in one line.
6. completes its work.

**Parent Process:**
1. synchronize the work of Parent processes using a semaphore
2. to implement message transmission using events
3. Only one active process transmits Boss messages, the transmission of other messages from other
processes should be temporarily blocked using a semaphore;
4. Requests messages consisting of "A" from the console and passes them (one at a time) to the Boss process;
5. completes its work.
6. Accepts from the Boss process about the completion of the work

**The Child process:**
1. synchronize the work of Child processes using a mutex
2. to implement message transmission using events
3. only one active process transmits Boss messages, the transmission of other messages from other
processes should be temporarily blocked using a mutex;
4. Requests messages consisting of "B"" from the console and passes them (one at a time) to the Boss process;
5. completes its work.
6. Accepts from the Boss process about the completion of the work

- - -

### Code Explanations:

#### The Child process:

1. main Function:

``` cpp
int main(int argc, char *argv[])
{
    // ...
}
```
The main function receives command-line arguments argc and argv. The child process reads the arguments and initializes the synchronization mechanisms.

2. Opening Mutex and ChildEvent:

``` cpp
hMutex = OpenMutexW(SYNCHRONIZE, FALSE, L"Mutex");
// Error handling
hEvent = OpenEventW(EVENT_MODIFY_STATE, FALSE, L"ChildEvent " + atoi(argv[1]));
// Error handling
```

3. Processing Events:

``` cpp
string buffer;
for (int i = 0, msg, nMsg = atoi(argv[0]); i < nMsg; i++)
{
    WaitForSingleObject(hMutex, INFINITE);
    cout << "Event " << i + 1 << endl;
    cin >> buffer;
    SetEvent(hEvent);
    ReleaseMutex(hMutex);
}
```
The child process enters a loop that iterates based on the argument argv[0]. In each iteration, the process waits for the mutex, reads user input, sets the corresponding "ChildEvent", and releases the mutex.

4. Setting ChildEndedEvent and Cleanup:

``` cpp
SetEvent(hEndedEvent);

CloseHandle(hMutex);
CloseHandle(hEvent);
CloseHandle(hEndedEvent);
return 0;
```
After processing all events, the child process sets the "ChildEndedEvent" to notify the parent process. It then closes the handles for the mutex and events and returns 0.

#### Parent Process:
1. main Function:

``` cpp
int main(int argc, char *argv[])
{
    // ...
}
```
The main function receives command-line arguments argc and argv. The parent process reads the arguments and initializes the synchronization mechanisms.

2. Opening Semaphore and ParentEvent:
```
hSemaphore = OpenSemaphoreW(SEMAPHORE_MODIFY_STATE | SYNCHRONIZE, FALSE, L"Semaphore");
// Error handling
hEvent = OpenEventW(EVENT_MODIFY_STATE, FALSE, L"ParentEvent " + atoi(argv[1]));
// Error handling
```
3. Processing Events:
``` cpp
string buffer;
for (int i = 0, nMsg = atoi(argv[0]); i < nMsg; i++)
{
    WaitForSingleObject(hSemaphore, INFINITE);
    cout << "Event " << i + 1 << endl;
    cin >> buffer;
    SetEvent(hEvent);
    ReleaseSemaphore(hSemaphore, 1, NULL);
}
```
The parent process enters a loop that iterates based on the argument argv[0]. In each iteration, the process waits for the semaphore, reads user input, sets the corresponding "ParentEvent", and releases the semaphore.
4. Setting ParentEndedEvent and Cleanup:
``` cpp
SetEvent(hEndedEvent);

CloseHandle(hSemaphore);
CloseHandle(hEvent);
CloseHandle(hEndedEvent);
return 0;
```
After processing all events, the parent process sets the "ParentEndedEvent" to notify the other process. It then closes the handles for the semaphore and events and returns 0.

#### Boss Process:

1. Creating Mutex and Semaphore:
``` cpp
HANDLE hMutex = NULL;
hMutex = CreateMutexW(NULL, FALSE, L"Mutex");
// Error handling

HANDLE hSemaphore = NULL;
hSemaphore = CreateSemaphoreW(NULL, 2, 2, L"Semaphore");
// Error handling
```
The program creates a mutex and a semaphore with initial and maximum counts of 2.
2. User Input for Number of Parent and Child Processes:
``` cpp
int nParentPr;
int nChildPr;
int nParentMsg;
int nChildMsg;
int nSumMsg = 0;

cout << "Enter the number of Parent processes: ";
cin >> nParentPr;
cout << "Enter the number of Child processes: ";
cin >> nChildPr;
```
The program prompts the user to input the number of parent and child processes.
3. Creating Parent and Child Processes:
``` cpp
// ...
for (int i = 0; i < nParentPr; i++)
{
    // ...
    if (!CreateProcessW(L"parent.exe", lpszCommandLine, NULL, NULL, FALSE,
        CREATE_NEW_CONSOLE, nullptr, NULL, siParent[i], piParent[i]))
    {
        // Error handling
    }
}
for (int i = 0; i < nChildPr; i++)
{
    // ...
    if (!CreateProcessW(L"child.exe", lpszCommandLine, NULL, NULL, FALSE,
        CREATE_NEW_CONSOLE, nullptr, NULL, siChild[i], piChild[i]))
    {
        // Error handling
    }
}
```
Two loops are used to create the parent and child processes. A new process is created for each parent and child with their respective executable files (parent.exe and child.exe).
4. Processing Messages from Parent and Child Processes:
``` cpp
for (int i = 0; i < nSumMsg + nSumPr; i++)
{
    // ...
}
```
The program enters a loop that iterates based on the total number of messages and processes. It prints messages and the termination status of each parent and child process.
5. Cleaning Up Resources:
``` cpp
// ...
for (int i = 0; i < nParentPr; i++)
{
    // ...
}
for (int i = 0; i < nChildPr; i++)
{
    // ...
}

CloseHandle(hMutex);
CloseHandle(hSemaphore);
return 0;
```
After processing all messages and processes, the program cleans up resources by closing the handles for mutexes, semaphores, and processes.
