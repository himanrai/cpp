/*README:
 * We will learn about:
 * [1] std::this_thread::get_id(); //get this thread id
 * [2] std::thread::hardware_concurrency(); //max number of threads supported
 * [3] std::thread t1(func1, msg); //thread creation
 * [4] t1.get_id(); //get t1's thread id
 * [5] t1.join(); //join a thread i.e., i.e., wait for a thread
 * [6] t1.detach(); //dont wait for thread termination, kernel will reclaim the
 *                  //thread resources
 * */

#include <iostream>
#include <thread>
#include <string>


void func1(std::string msg) {
    std::cout << "func1: " << msg << std::endl;
}

void func2(std::string& msg) {
    std::cout << "func2: " << msg << std::endl;
    msg = "Msg from func2"; //update the msg
}

class Functor1 {
public:
    void operator()(std::string& msg) {
        std::cout << "Functor1: " << msg << std::endl;
        msg = "Msg from Functor1"; //update the msg
    }
};

class Functor2 {
public:
    void operator()(std::string&& msg) {
        std::cout << "Functor2: " << msg << std::endl;
    }
};



int main()
{
    std::string msg = "Msg1 from main";

    std::cout << "main thread-id: " << std::this_thread::get_id() << std::endl;
    //How many threads can be concurrently run by the program?
    std::cout << std::thread::hardware_concurrency() << std::endl;
    std::cout << std::endl;

    std::thread t1(func1, msg); //pass by value
    std::cout << "t1 thread-id: " << t1.get_id() << std::endl;
    t1.join();
    std::cout << std::endl;


    msg = "Msg2 from main";
    std::thread t2(func2, std::ref(msg)); //pass by reference
    t2.join();
    std::cout << "main: " << msg << std::endl;
    std::cout << std::endl;

    msg = "Msg3 from main";
    Functor1 functor1;
    std::thread t3(functor1, std::ref(msg)); //pass by reference
    t3.join();
    std::cout << "main: " << msg << std::endl;
    std::cout << std::endl;

    msg = "Msg4 from main";
    Functor2 functor2;
    std::thread t4(functor2, std::move(msg));
    t4.join();
    std::cout << std::endl;

    msg = "Msg5 from main";
    std::thread t5(func1, msg);
    std::thread t6 = std::move(t5); //thread can only be moved, not copied
    t6.join();
    std::cout << std::endl;

    return 0;
}
