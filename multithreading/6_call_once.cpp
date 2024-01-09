/*README:
 * We will learn about:
 * - Lazy Initialization
 * - std::once_flag and std::call_once()
 *
 * [1] If we open a file in constructor, and never call .log method
 *     then resources to open a file will be wasted.
 * [2] To avoid this we can open the file in .log method itself during
 *     the first time only. This is called "Lazy Initialization" or 
 *     "Initialization Upon First Use Idiom".
 * [3] Now to make open thread-safe we need to lock it. To do this we will use
 *     another mutex mu2. We dont want to use mu because mu will be locked for
 *     each writing and we want to lock mu2 only for the first time before opening
 *     the file.
 *     But with this program is not thread-safe. Lets say we have 2 threads
 *     t1 and t2:
 *     - t1 executes, finds that file is not open, takes the lock mu2 and suspends.
 *     - t2 executes, finds that file is not open, waits for the lock mu2 and
 *       suspends.
 *     - t1 executes, opens the file, unlock mu2, lock mu, writes data into
 *       file, unlock mu.
 *     - t2 executes, opens the file again.
 *     So the porgram is not thread-safe.
 * [4] So, along with ofs.open(), ofs.is_open() also needs to be synchronized.
 *     - Lets take the lock before ofs.is_open().
 *     - This introduces another problem:
 *       The file needs to be opened only once, but everytime .log method is
 *       called then mu2 is locked, check if file is open and then unlock mu2.
 *       This locking and unlocking is waste of computer cycles. 
 * [5] Standard library provides a solution to this specific problem.
 *     - Instead of using mu2, we will use std::once_flag and std::call_once()
 *
 *
 * */

#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <fstream>
#include <chrono>

using namespace std;

#define MAX_LOOP 5

/* ofs and cout are the shared resources
 * */
class Logger
{
private:
    ofstream ofs;
    mutex mu;
    //mutex mu2; //[4]
    std::once_flag oflag; //[5]
public:
    Logger() {
        //ofs.open("log.txt"); //[1]
    };
    ~Logger() {
        if (ofs.is_open()) {
            ofs.close();
        }
    }

    void log(string msg) {
        #if 0 //[4]
        {
            unique_lock<mutex> locker2(mu2); //[4]
                if (!ofs.is_open()) {
                    //unique_lock<mutex> locker2(mu2); //[3]
                    ofs.open("log.txt"); //[2]
                }
        }
        #endif
        //file will be opened only once by one thread
        //             once_flag, lambda
        std::call_once(oflag, [&](){ofs.open("log.txt");}); //[5]

        unique_lock<mutex> locker(mu, std::defer_lock);

        locker.lock();
        if (ofs.is_open()) {
            ofs << msg;
        } else {
            cout << msg;
        }
    }

};


void thread1(Logger& logger)
{
    string msg;
    for (int i=0; i<MAX_LOOP; i++) {
        //std::this_thread::sleep_for(std::chrono::milliseconds(10));
        msg = "thread1: " + to_string(i) + "\n";
        logger.log(msg);
    }
}

void thread2(Logger& logger)
{
    string msg;
    for (int i=0; i>-MAX_LOOP; i--) {
        //std::this_thread::sleep_for(std::chrono::milliseconds(20));
        msg = "thread2: " + to_string(i) + "\n";
        logger.log(msg);
    }
}

int main()
{
    Logger logger;
    std::thread t1(thread1, std::ref(logger));
    std::thread t2(thread2, std::ref(logger));

    t1.join();
    t2.join();

    cout << "main() done" << endl;

    return 0;
}
