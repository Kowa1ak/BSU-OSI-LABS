# Laboratory work No. 2 

## Process creation

- - -

### Task:

1. Store two projects (processes) in one Solution (Solution) in VS Studio!
2. In Solution (Solution and) configure to .the exe files were in one Debug!
3. Explain what data the structures store: STARTUPINFO, PROCESS_INFORMATION.
4. Write programs for two console processes Parent and Child that execute.

Parent Process:

* Enter the size of the array, enter the elements of the array;
* Generates a command line that contains information about the dimension of the array, elements, etc.(according to the individual option);
* Sets visual settings for the child process console, according to individual options:

Установить высоту (Y) смещения от верхнего левого угла экрана.

- - -

### Code Explanations:

1. Entering an array:

```
int size = 0;
cout << "Enter size of array:  ";
cin >> size;
int* array = new int[size];
for (int i = 0; i < size; i++) {
    cout << "Enter data for array[" + to_string(i) + "]: ";
    cin >> array[i];
}
```

Here the program asks the user for the size of the array and its elements.

2. Formation of the command line:

```
int j = 0;
string temp = "";

for (int i = 0; i < size; i++) {
    temp += std::to_string(array[i]);
    if (i != size - 1) {
        temp += " ";
    }
}
char* command_line = new char[temp.length() + 1];
strcpy(command_line, temp.c_str());

```

Here, the array elements are combined into a single line with space delimiters, and then the string is converted into an array of char characters, which will be used as a command line for the child process.

3. Creating and launching a child process:

```
char child_process_name[] = "C:\\Danik\\OSI\\Lab2\\Debug\\Lab2.exe";
STARTUPINFO startup_info;
PROCESS_INFORMATION process_information;

ZeroMemory(&startup_info, sizeof(STARTUPINFO));
startup_info.cb = sizeof(STARTUPINFO);
startup_info.dwFlags = STARTF_USEPOSITION;
startup_info.dwY = 500;
```
Here the STARTUPINFO and PROCESS_INFORMATION structures are initialized, and the path to the executable file of the child process is also set.

```
if (CreateProcess(child_process_name, command_line, NULL,
    NULL, false, CREATE_NEW_CONSOLE,
    NULL, NULL, &startup_info, &process_information)) {

    WaitForSingleObject(process_information.hProcess, INFINITE);
    CloseHandle(process_information.hProcess);
    CloseHandle(process_information.hThread);
}
else {
    cout << "Error";
    return 0;
}
```
Here a new child process is created and started using the CreateProcess function, passing it the path to the executable file, the command line and the STARTUPINFO structure. If the process is successfully created, the program waits for its completion and closes the process and thread descriptors. If the process could not be created, an error message is displayed.
