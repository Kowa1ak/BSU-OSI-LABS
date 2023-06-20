# Laboratory work No. 1

A task. Write a program for a console process that consists of two threads: main and worker.

- - -

* The **main** thread should do the following:

1. Create an array of integers, the dimension and elements of which are entered from the console (or generate randomly).
2. Enter the time to stop and start the worker thread.
3. Create a worker thread, transfer data to the stream: array size, masiv, etc.
4. Suspend the **worker** thread (SuspendThread), then after a while start the thread again.
5. Be able to create a stream with the \_beginthreadex command
6. Wait for the worker thread to finish.
7. Output the result of the worker thread to the console
8. Finish the job.

* Do not use global variables!
* Explain: what is an identifier, a descriptor, the operation of functions.
* The worker thread should do the following work:
Output the number of elements multiples of 5 from the array elements. Finish your work.

- - -

### Code Explanations:

1. Entering an array:

```
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
```

Here the program asks the user for the size of the array and its elements. The array is created with an additional element to store the size.

2. Creating a stream:

```
HANDLE hThread;
DWORD IDThread;
hThread = CreateThread(NULL, 0, worker, (void*)array, 0, &IDThread);
if (hThread == NULL) {
return GetLastError();
}
```
Here, a new thread is created using the CreateThread function, passing the worker function to it as an entry point and an array of data. If the creation of the stream failed, the program terminates with an error.

3. Worker function:

```
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
```
The worker function represents the entry point for a new thread. It iterates through the elements of the array and counts the number of numbers that are multiples of 5. The result is displayed on the screen.

4. Flow control:

```
SuspendThread(hThread);
Sleep(5000);
ResumeThread(hThread);
WaitForSingleObject(hThread, INFINITE);
CloseHandle(hThread);
```
Here the program suspends the thread using SuspendThread, waits 5 seconds (Sleep(5000)), resumes the thread using ResumeThread, waits for it to finish (WaitForSingleObject) and closes the thread handle (CloseHandle).