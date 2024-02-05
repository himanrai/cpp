/*README:
 * We will learn about:
 * - std::packaged_task() function
 *
 * [1] packaged_task() is implemented in different ways in below functions:
 *     main1(), main2(), ...
 *     Please refer these functions.
 *
 * */


#include <iostream>     // std::cout
#include <future>       // std::packaged_task, std::future
#include <chrono>       // std::chrono::seconds
#include <thread>       // std::thread, std::this_thread::sleep_for
#include <functional>   // std::bind
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable
#include <deque>

using namespace std;

int factorial(int N) {
    int result = 1;
    for (int i=N; i>1; i--) {
        result *= i;
    }
    //cout << "Result is: " << result << endl;
    return result;
}

void main1();
void main2();
void main3();
void main4();
void main5();
void main6();

// Driver Code
int main()
{
    cout << "main(): " << factorial(5) << endl;
    main1();
    main2();
    main3();
    main4();
    main5();
    main6();
    return 0;
}

void main1() {
    /**
     * The class template std::packaged_task wraps any Callable target (function, lambda expression,
     * bind expression, or another function object) so that it can be invoked asynchronously.
     * Its return value or exception thrown is stored in a shared state which can be accessed
     * through std::future objects.
     **/
    std::packaged_task<int(int)> task(factorial);
    std::future<int> fu = task.get_future();

    //do something

    //excute task at later point
    task(5);
    cout << "main1(): " << fu.get() << endl;
    return;
}

void main2() {
    std::packaged_task<int(int)> task(factorial);
    task(5);
    cout << "main2(): " << task.get_future().get() << endl;
    return;
}

//Task can be executed in other context
void main3() {
    std::packaged_task<int(int)> task(factorial);
    std::future<int> fu = task.get_future();
    std::thread t1(std::move(task), 5);
    cout << "main3(): " << fu.get() << endl;
    t1.join();
    return;
}

void main4() {
    std::packaged_task<int()> task(std::bind(factorial, 5));
    std::future<int> fu = task.get_future();
    task();
    cout << "main4(): " << fu.get() << endl;
    return;
}

/**
 * - Using function object (i.e., fntor) we are able to invoke callable.
 *   However, we cannot get a future from it.
 * - So, the advantage of the packaged_task() is that it can link a callable
 *   object to a future which is imp in threading environment.
 **/
void main5() {
    auto fntor = std::bind(factorial, 5);
    cout << "main5(): " << fntor() << endl;
    return;
}

std::deque<packaged_task<int()>> task_q;
std::mutex mu;
std::condition_variable cond;

void thread_1() {
    std::packaged_task<int()> task;
    
    {
        std::unique_lock<std::mutex> locker(mu);
        cond.wait(locker, [](){return !task_q.empty();});
        task = std::move(task_q.front());
        task_q.pop_front();
    }
    task();
}

void main6() {
    std::thread t1(thread_1);

    std::packaged_task<int()> task(std::bind(factorial, 5));
    std::future<int> fu = task.get_future();

    {    
        std::lock_guard<std::mutex> locker(mu);
        task_q.push_back(std::move(task));
    }
    cond.notify_one();
    
    cout << "main6(): " << fu.get() << endl;
    t1.join();

    return;
}

/**
 * Summary:
 * There are 3 ways to get a future:
 *  1. std::promise::get_future();
 *  2. std::packaged_task::get_future();
 *  3. std::async returns a future
 **/
