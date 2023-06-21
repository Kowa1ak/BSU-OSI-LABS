# Laboratory work No. 6

#### Creating and synchronizing threads in the c++ 11 standard of mutexes and condition_variable(or condition_variable_any), binary semaphore(c++20 standard)

- - -

### Task:
Implement Lab 3 in c++11 (c++11 standard):
1. Use std::thread to create threads and work with them.
2. Implement events via std::condition_variable_any (or std::condition_variable) and std::mutex
3. Instead of the critical section (win32) - use std::mutex or std::binary_semaphore (C++20)

- - -

### Code Explanations:

1. The work function:

``` cpp
void work(int sleepTime) {
    std::unique_lock<std::mutex> lock(mtx);

    int left = 0;
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] == X) {
            std::swap(arr[left], arr[i]);
            left++;
        }
    }

    for (int i = 0; i < arr.size(); i++) {
        std::cout << arr[i] << " ";
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    }
    std::cout << std::endl;
}
```
This function is executed in one thread. It uses a mutex (mtx) to protect the shared data (arr) while performing operations. It swaps elements equal to X to the left side of the vector and then prints the modified vector.

2. The CountElement function:

``` cpp
void CountElement() {
    std::unique_lock<std::mutex> lock(mtx);

    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] == X) count_k++;
    }

    cv.notify_one();
}
```
This function is executed in another thread. It also uses the mutex (mtx) to protect shared data (arr and count_k). It counts the number of elements equal to X in the vector.

3. The main function:

``` cpp
int main() {
    // ...

    std::thread thread1;
    std::thread thread2;

    // ...

    thread1 = std::thread(work, sleepTime);
    thread2 = std::thread(CountElement);

    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock);
    }

    // ...

    thread1.join();
    thread2.join();

    return 0;
}
```
In the main function, the user is prompted to enter the size of the array (n), the array elements, a character (X), and a sleep interval. Two threads are created, thread1 for executing the work function and thread2 for executing the CountElement function.

A unique lock is used to protect the shared data (mtx) during the cv.wait() call, which waits until the cv.notify_one() is called in the CountElement function. This synchronization ensures that the count is obtained only after the work function has finished modifying the vector.

Finally, the counts are printed, and the main thread waits for both threads to finish executing (thread1.join() and thread2.join()).
