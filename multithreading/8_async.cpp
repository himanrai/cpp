/*README:
 * We will learn about:
 * - std::async() function
 * - std::future<> class
 * - std::promise<> class
 *
 * [1] Parent thread gets a value from Child thread
 *     see notes below.
 * [2] std::async() function and std::future class
 *     see notes below.
 * [3] std::launch::deferred and std::launch::async parameters
 *     see notes below .
 * [4] std::promise<int> -> std::future<int>
 *     Parent thread sends a value to Child thread.
 *     see notes below.
 * [5] std::shared_future<int>
 *     see notes below.
 *
 * */

#include <iostream>
#include <thread>
#include <mutex>
#include <future>
#include <chrono>

using namespace std;

void factorial1(int n) {

    int fact = 1;

    for(int i=n; i>1; i--) {
        fact *= i;
    }

    cout << "factorial1(): " << fact << endl;
}

void factorial2(int n, int& result) {

    int fact = 1;

    for(int i=n; i>1; i--) {
        fact *= i;
    }   

    cout << "factorial2(): " << fact << endl;
    result = fact;
}

int factorial3(int n) {

    int fact = 1;

    for(int i=n; i>1; i--) {
        fact *= i;
    }   

    cout << "factorial3(): " << fact << endl;
    return fact;
}

int factorial4(std::future<int>& fut) {

    int fact = 1;

    //fu.get() will wait for prom.set_value()
    int n = fut.get();
    /* - What if parent thread forgets to keep up its promise and dont call
     *   prom.set_value(3);?
     *   In such case fut.get(), in child thread, will throw exception
     *   std::future_errc::broken_promise
     * */

    for(int i=n; i>1; i--) {
        fact *= i;
    }   

    cout << "factorial4(): " << fact << endl;
    return fact;
}

int factorial5(std::shared_future<int> sfut) {

    int fact = 1;

    //sfu.get() will wait for prom.set_value()
    int n = sfut.get();

    for(int i=n; i>1; i--) {
        fact *= i;
    }   

    cout << "factorial5(): " << fact << endl;
    return fact;
}


int main()
{
#if 0
    std::thread t1(factorial1, 4);
    t1.join();

    /* [1] 
     * - In order to get the result from a thread we need to pass a variable by
     *   reference.
     * - And since result is a shared resource between t1 and main, we will
     *   have to lock it with mutex.
     * - Also, we need to make sure that result should be set by t1 before main
     *   tries to access it and for this we would need a condition variable.
     * - Just to get a value from thread we will have to do all these things and
     *   on top of that we will have to manage a global mutex and
     *   a condition_variable.
     * - CPP standard prrovides a better solution: std::async()
     * */
    int result;
    std::thread t2(factorial2, 4, std::ref(result));
    t2.join();
    cout << "main(): " << result << endl;
#endif


    /* [2] 
     * - std::async() is a function, whereas std::thread is a class.
     * - std::async() creates a thread and returns a future object that
     *   act as a channel to get the result from the child thread.
     * - fu.get() will wait until child thread finish and then return the
     *   returned value from child thread. 
     * - std::future class represents an object using which you can get
     *   something in future.
     * - future object (i.e., fu) can call the .get() method only once.
     * */
     std::future<int> fu = std::async(factorial3, 4);
     int x = fu.get();
     cout << x << endl;



    /* [3] 
     * - std::async() actually may or may not create a thread and that can be
     *   controlled by another parameter.
     * - std::launch::deferred
     *   With this parameter std::async() will defer the exdcution of thread
     *   until fu.get() method is called.
     * - std::launch::async
     *   With this parameter std::async() will create a thread.
     * - std::launch::deferred | std::launch::async
     *   With this parameter std::async() may or may not create a thread. This
     *   is default behavior.
     * - 
     * */
     std::future<int> fu1 = std::async(std::launch::deferred, factorial3, 4);
     int y = fu1.get();
     cout << y << endl;

     std::future<int> fu2 = std::async(std::launch::async, factorial3, 4);
     int z = fu2.get();
     cout << z << endl;


    /* [4] 
     * - We can use std::future to send value from parent thread to child
     *   thread, not at the time of creating the thread but sometime in future.
     * - For this we would need a std::promise object and another std::future
     *   object.
     * - parent thread sends the std::future object (i.e., fut) to child thread via
     *   std::aync()
     * - parent thread keeps up its promise by calling prom.set_value(3); to
     *   send value to child thread.
     * - child thread waits on fut.get() to get the value from parent thread. 
     * - What if parent thread forgets to keep up its promise and dont call
     *   prom.set_value(3);?
     *   For such case fut.get(), in child thread, will throw exception
     *   std::future_errc::broken_promise
     * - In case, parent thread cannot call prom.set_value(3); then it should
     *   set an exception:
     *     prom.set_exception(std::make_exception_ptr(std::runtime_error("Cannot
     *                        keep up the promise")));
     *   Now in child thread, the fut.get() will get this facncy exception
     *   rather than std::future_errc::broken_promise exception.
     * - Note that neither future nor the promise can be copied. They can only
     *   be std::move just like the std::thread and std::unique_lock<int>
     * - 
     * */
     std::promise<int> prom;
     std::future<int> fut = prom.get_future();
     std::future<int> fu3 = std::async(std::launch::async, factorial4, std::ref(fut));
     //do something else
     std::this_thread::sleep_for(std::chrono::milliseconds(50));
     prom.set_value(3);
     int a = fu3.get();
     cout << a << endl;




    /* [5] 
     * - If we want mutliple threads of factorial and want send to data from
     *   parent thread to those children threads using promise->future
     *   channel then we either need to create multiple promise->future pair
     *   which is inappropriate or we need to have a shared_future.
     * - Unlike "std::future<int> fut1;" object, "std::shared_future<int>
     *   sfut1" object can be copied.
     * - And when we call prom1.set_value(5);, all the copies of sfut1 objects
     *   will get the value via sfut1.get() inside each child thread.
     * */
     std::promise<int> prom1;
     std::future<int> fut1 = prom1.get_future();
     std::shared_future<int> sfut1 = fut1.share();
     std::future<int> fu4 = std::async(std::launch::async, factorial5, sfut1);
     std::future<int> fu5 = std::async(std::launch::async, factorial5, sfut1);
     std::future<int> fu6 = std::async(std::launch::async, factorial5, sfut1);
     prom1.set_value(5);
     int b, c, d;
     b = fu4.get();
     c = fu5.get();
     d = fu6.get();
     cout << b << " " << c << " " << d << " " << endl;
    
 
    return 0;
}
