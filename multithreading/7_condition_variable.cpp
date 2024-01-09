/*README:
 * We will learn about:
 * - Busy waiting state
 * - std::condition_variable
 * - spurious wake: cond.wait(locker);
 * - predicate: cond.wait(locker, pedicate);
 *
 * [1] producer1() -> consumer1()
 *     consumer1() is in a busy waiting state. It takes the lock, checks the
 *     queue, if it is empty it unlocks the locker. And immediately gets into
 *     the next loop. This busy waiting is inefficient.
 * [2] producer1() -> consumer2()
 *     To make consumer1() efficient, we can put it to sleep if the queue is empty
 *     (see consumer2()) to reduce the number of looping. The problem how do we
 *     decide about the time duration of it nap.
 *     - If the time is too short then the consumer2() still end up spending lot
 *       of time in looping.
 *     - If the time is too long then it may not be able to consume the data in
 *       time.
 *     - It is difficult to find the right time to sleep, this is where the
 *       condition variable comes handy.
 * [3] producer3() -> consumer3()
 *     -> cond.notify_one()
 *     -> cond.wait(locker);
 *        - This will put consumer3() into sleep, until it is notified by
 *          producer3()
 *        - condition_variable can enforce that consumer3() can go ahead and
 *          fetch the data only after producer3() pushed the data into the
 *          queue. In other words condition_variable can enfore certain parts of
 *          two threads to be executed in a predefined order.
 *        - why do we need locker with cond.wait(locker)?
 *          Before the wait() method put the consumer3() thread into sleep, it will
 *          unlock the locker and then go to sleep. And once consumer3() is
 *          notified, wait() will lock the locker again and then continue to
 *          access the queue.
 *        - Since we will have to lock and unclock the mutex, we will have to
 *          use unique_lock() with condition_variable, we  cannot use
 *          lock_guard().
 *        - It will work fine as long as consumer3(), while in waiting state
 *          (cond.wait(locker)), can only be woke up by the cond.notify_one().
 *          But that is not the case. In fact cond.wait(locker) can wake up by
 *          itself and that is called spurious wake.
 *        - And if it is a spurious wake we dont want consumer3() to run, we
 *          want it to sleep again. So the cond.wait() can take another
 *          parameter which is a predicate that determine whether the condition
 *          is really met for the thread to continue running. Here we will use
 *          a lambda function for the predicate.
 *     -> cond.wait(locker, [](){ return !dq.empty(); });
 *        - If consumer3() thread wakes up and found that queue is empty, it
 *          will go to sleep again. If the queue is not empty it will go ahead
 *          and pop the data.
 *     -> Another point to note that there could be more that one thread
 *        waiting on a condition. With cond.notify_one() only one thread will
 *        be notified. If you want all threads to wake up, you should call
 *        cond.notify_all().
 *     
 *
 * */

#include <iostream>
#include <deque>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>

std::deque<int> dq;
std::mutex mu;
std::condition_variable cond;

/******** producer1() -> consumer1() or consumer2() ************/
void producer1()
{
    int i = 10;
    while (i) {
        std::unique_lock<std::mutex> locker(mu);
        dq.push_back(i);
        locker.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        i--;
    }

    return;
}

void consumer1()
{
    int data = 0;
    while (data != 1) {
        std::unique_lock<std::mutex> locker(mu);
        if (!dq.empty()) {
            data = dq.front();
            dq.pop_front();
            locker.unlock();
            std::cout << "consumed: " << data << std::endl;
        } else {
            locker.unlock(); 
        }
    }

    return;
}

void consumer2()
{
    int data = 0;
    while (data != 1) {
        std::unique_lock<std::mutex> locker(mu);
        if (!dq.empty()) {
            data = dq.front();
            dq.pop_front();
            locker.unlock();
            std::cout << "consumed: " << data << std::endl;
        } else {
            locker.unlock(); 
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }   

    return;
}


/******** producer3() -> consumer3() ************/
void producer3()
{
    int i = 10; 
    while (i) {
        std::unique_lock<std::mutex> locker(mu);
        dq.push_back(i);
        locker.unlock();

        //condition_variable: Notify one thread, if there is one
        cond.notify_one();

        std::this_thread::sleep_for(std::chrono::seconds(1));
        i--;
    }   

    return;
}

void consumer3()
{
    int data = 0;
    while (data != 1) {
        std::unique_lock<std::mutex> locker(mu);
        //This will put consumer3() into sleep, until it is notified
        //cond.wait(locker); //spurious wake
        cond.wait(locker, [](){ return !dq.empty(); });

        data = dq.front();
        dq.pop_front();
        locker.unlock();
        std::cout << "consumed: " << data << std::endl;
    }
    
    return;
}



int main() {
    //std::thread t1(producer1);
    //std::thread t2(consumer1);
    //std::thread t2(consumer2);

    std::thread t1(producer3);
    std::thread t2(consumer3);

    t1.join();
    t2.join();

    return 0;
}
