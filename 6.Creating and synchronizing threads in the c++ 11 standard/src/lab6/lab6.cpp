#include <iostream>
#include <vector>
#include <thread>
#include <condition_variable>
#include <mutex>

std::mutex mtx;
std::condition_variable cv;
std::vector<char> arr;
char X;
int count_k = 0;

void work(int sleepTime) {
    std::unique_lock<std::mutex> lock(mtx);
    // std::cout << "th w\n";

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

void CountElement() {
    std::unique_lock<std::mutex> lock(mtx);
    // std::cout << "поток count_k";

    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] == X) count_k++;
    }

    // std::cout << " end поток count_k";
    cv.notify_one();
}

int main() {
    int n;
    std::cout << "Enter array size: ";
    std::cin >> n;

    arr.resize(n);
    std::cout << "Enter array elements: ";
    for (int i = 0; i < n; i++) std::cin >> arr[i];

    std::cout << "Array size: " << n << std::endl;
    std::cout << "Array elements: ";
    for (int i = 0; i < n; i++) std::cout << arr[i] << " ";
    std::cout << std::endl;

    std::thread thread1;
    std::thread thread2;

    std::cout << "Enter symbol X:";
    std::cin >> X;
    std::cout << "Enter the sleep interval (in milliseconds): ";
    int sleepTime;
    std::cin >> sleepTime;
    thread1 = std::thread(work, sleepTime);
    thread2 = std::thread(CountElement);

    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock);
    }

    std::cout << "Number of elements equal to " << X << ": " << count_k << std::endl;

    thread1.join();
    thread2.join();

    return 0;
}