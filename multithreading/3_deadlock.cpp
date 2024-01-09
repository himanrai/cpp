/*README:
 * We will learn about:
 * [1] deadlock
 *     When threads try to lock 2 mutexes in different order
 *
 * */

#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <fstream>
#include <chrono>

using namespace std;

#define MAX_LOOP 50

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
        lock_guard<mutex> locker1(mu2); //lock order: mu2->mu1
        lock_guard<mutex> locker2(mu1);
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
