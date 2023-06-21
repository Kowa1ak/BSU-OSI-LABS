# Laboratory work No. 3

## Thread Synchronization

- - -

### Task:

Write a program for a console process that consists of three threads: main, work, and the third.

#### Synchronization Objects :

Critical section- synchronization of work and CountElement flow (signals the start of calculations in CountElement);
Event - sets the CountElement stream for the main stream (to output the CountElement result to main ).
The main thread should perform the following actions:

1. Initialize the necessary events and critical sections.
2. create an array with symbol elements, the dimension and elements of which are entered by the user from the console;
3. output the dimension and elements of the source array to the console;
4. start the work thread (in limbo);
5. create a CountElement thread(in limbo);
6. Request the X symbol.
7. start the work flow;
8. start the CountElement stream;
9. display the result of the CountElement stream;
The work thread should perform the following actions:
1. request from the user the time interval required for rest after preparing one element in
the array;
2. search in the array of elements =X (place them in the array on the left, the remaining elements of the array on the right).
Elements are symbols. Enter X in the main thread.
3. display the elements of the array (final) element by element;
4. after each finished element, rest for a specified time interval;
5. notify the CountElement thread about the start of work (the start time will occur after the final array is formed (use the critical section)).

The CountElement thread must perform the following actions (To synchronize with the work thread - use
the critical section, with the main thread - the event!):

1. waits for the work thread to signal the beginning of summation (use the critical section) ;
2. count the number of elements equal to X;
3. notify (use an event) the main thread about the output of the result

- - -

### Code Explanations:

1. work Function:

``` cpp

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
```
This function is executed by the first thread. It moves all occurrences of X to the beginning of the array and prints the array with a sleep time between each character.

2. CountElement Function:

``` cpp
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
```

This function is executed by the second thread. It counts the occurrences of X in the array and sets the event when it's done.

3. Main Function:

``` cpp
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
```
The main function initializes the critical section and event, takes input from the user to populate the character array and the character X, and creates two suspended threads with work and CountElement functions. Then it resumes the threads, waits for the event to be set, and prints the count of X occurrences. After that, it waits for the threads to finish, closes handles, deletes the critical section, and returns 0.
