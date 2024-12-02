#include "c3/ex1.hpp"

// Write a program where multiple threads increment a shared counter. Use a std::mutex to protect the shared counter and ensure its value is updated correctly.
// 	-Challenge: Remove the mutex and observe the effects. Can you explain why the counter might not increment correctly?
// 	-Extra Credit: Use std::atomic<int> instead of a mutex. How does this compare in performance and correctness?

ScopedThread::ScopedThread(std::thread t): t_{std::move(t)}
{
    if(!t.joinable()){
        throw(std::logic_error("No thread!"));
    }
}

ScopedThread::~ScopedThread()
{
    if(t_.joinable()){
        t_.join();
    }
}

// move constructor so that we can put the scoped thread in a vector
ScopedThread::ScopedThread(ScopedThread&& other) noexcept: 
            t_{std::move(other.t_)}{

}

ScopedThread& ScopedThread::operator=(ScopedThread&& other) noexcept{
    if(this != &other){
        if(t_.joinable()){
            t_.join();
        }
        t_ = std::move(other.t_);
    }
    return *this;
}

struct counter{
    int counter_{0};
    std::mutex mx_;
};

void IncrementCounter(counter& c){
    for(int i = 0; i < 100; ++i){
        std::lock_guard<std::mutex> lg{c.mx_};
        c.counter_++;
        std::cout << "Thread " << std::this_thread::get_id() << " set "
                  << "the counter to " << c.counter_ << "\n";
    }
}

int ex1(){
    counter c{};
    // std::vector<ScopedThread> vec{};

    ScopedThread s1{std::move(std::thread{IncrementCounter, std::ref(c)})};
    ScopedThread s2{std::move(std::thread{IncrementCounter, std::ref(c)})};

    // for(int i = 0; i < 10; ++i){
    //     vec.push_back(ScopedThread{
    //                   std::move(std::thread{IncrementCounter, std::ref(c)})
    //                   });
    // }
    return 0;
}