/*README:
 * We will learn about:
 * [1] deadlock avoidance.
 *
 * */

/* To avoid deadlock make sure:
 * [1] Prefer locking single mutex.
 *     {
 *         lock_guard<mutex> locker1(mu1);
 *     }//mu1 will be unlocked as it goes out-of-scope
 *     {
 *         lock_guard<mutex> locker2(mu2);
 *     }//mu2 will be unlocked as it goes out-of-scope
 * [2] Avoid locking a mutex and then calling a user function, which might try
 *     another lock or may try to lock the same mutex.
 * [3] Use std::lock() to lock more than one mutex.
 * [4] Lock the nutex in same order
 * [5] Provide an hierarchy of mutex so that if a thread is holding lower level
 *     of mutex, it is not allowed to lock a higher level mutex.
 * */

#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <fstream>
#include <chrono>

using namespace std;

#define MAX_LOOP 1000

/* [1] ofs and cout are the shared resources
 * [2]
 * */
class Logger
{
private:
    ofstream ofs;
    mutex mu1;
    mutex mu2;
public:
    Logger() = default;
    Logger(string logfile) {
        ofs.open(logfile.c_str());
        if (!ofs.is_open()) {
            throw string("cannot open: ") + logfile;
        }
    }
    ~Logger() {
        if (ofs.is_open()) {
            ofs.close();
        }
    }


    //Enable any of the solution for deadloack avoidance
#if 0
    //SOLUTION#1: Lock the mutexes in same order
    void log1(string msg) {
        lock_guard<mutex> locker1(mu1); //lock order: mu1->mu2
        lock_guard<mutex> locker2(mu2);
        if (ofs.is_open()) {
            ofs << msg;
        } else {
            cout << msg;
        }
    }

    void log2(string msg) {
        lock_guard<mutex> locker1(mu1); //lock order: mu1->mu2
        lock_guard<mutex> locker2(mu2);
        if (ofs.is_open()) {
            ofs << msg;
        } else {
            cout << msg;
        }
    }
#endif

#if 0
    //SOLUTION#2: Use std::lock() - mutexes need not to be in same order
    void log1(string msg) {
        std::lock(mu1, mu2); //Lock the 2 mutex
        if (ofs.is_open()) {
            ofs << msg;
        } else {
            cout << msg;
        }
        mu1.unlock();
        mu2.unlock();
        
    }

    void log2(string msg) {
        std::lock(mu2, mu1); //Lock the 2 mutex
        if (ofs.is_open()) {
            ofs << msg;
        } else {
            cout << msg;
        }
        mu2.unlock();
        mu1.unlock();
    }
#endif

#if 1
    //SOLUTION#3: Use std::lock() - mutexes need not to be in same order
    void log1(string msg) {
        std::lock(mu1, mu2); //Lock the 2 mutex

        // mu1 is already locked using std::lock() above, std::adopt_lock 
        // is just adopt mu1 and unlock it once locker1 object goes out of scope
        lock_guard<mutex> locker1(mu1, std::adopt_lock);
        lock_guard<mutex> locker2(mu2, std::adopt_lock);
        if (ofs.is_open()) {
            ofs << msg;
        } else {
            cout << msg;
        }
    
    }   

    void log2(string msg) {
        std::lock(mu2, mu1); //Lock the 2 mutex
        lock_guard<mutex> locker1(mu1, std::adopt_lock);
        lock_guard<mutex> locker2(mu2, std::adopt_lock);
        if (ofs.is_open()) {
            ofs << msg;
        } else {
            cout << msg;
        }
    }   
#endif


};


void thread1(Logger& logger)
{
    string msg;
    for (int i=0; i<MAX_LOOP; i++) {
        //std::this_thread::sleep_for(std::chrono::milliseconds(10));
        msg = "thread1: " + to_string(i) + "\n";
        logger.log1(msg);
    }
}

void thread2(Logger& logger)
{
    string msg;
    for (int i=0; i>-MAX_LOOP; i--) {
        //std::this_thread::sleep_for(std::chrono::milliseconds(20));
        msg = "thread2: " + to_string(i) + "\n";
        logger.log2(msg);
    }
}

int main()
{
    Logger logger;
    //Logger logger("log.txt");
    std::thread t1(thread1, std::ref(logger));
    std::thread t2(thread2, std::ref(logger));

    t1.join();
    t2.join();

    cout << "main() done" << endl;

    return 0;
}
