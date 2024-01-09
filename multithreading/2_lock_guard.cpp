/*README:
 * We will learn about:
 * [1] mutex mu;
 *     mu.lock(); //See NOTE#1 below
 *     mu.unlock();
 * [2] lock_guard<mutex> locker(mu); //RAII
 *
 * */

#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <fstream>
#include <chrono>

using namespace std;

#define MAX_LOOP 10

/* [1] ofs and cout are the shared resources
 * [2]
 * */
class Logger
{
private:
    ofstream ofs;
    mutex mu;
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

    void log(string msg) {

        //mu.lock(); //takes the mu lock

        /* NOTE#1:
         * After mu.lock() and before mu.unlock(), if the code throws an
         * exception then mutex will be locked forever.
         * To avoid this lets use lock_guard() which is implemented as RAII.
         * */

        //RAII: mu will be auto unlock when lock_guard goes out-of-scope
        lock_guard<mutex> locker(mu); //takes the lock
        if (ofs.is_open()) {
            ofs << msg;
        } else {
            /* cout, is a shared resource that is not thread-safe despite being
             * protected by lock, because cout can be used anywhere in the
             * code. So, it is safer to use ofs.
             * */
            cout << msg;
        }

        //mu.unlock(); //unlock the mu
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
    //Logger logger("log.txt");
    std::thread t1(thread1, std::ref(logger));
    std::thread t2(thread2, std::ref(logger));

    t1.join();
    t2.join();

    cout << "main() done" << endl;

    return 0;
}
