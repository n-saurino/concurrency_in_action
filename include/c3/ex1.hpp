#pragma once
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <sstream>
#include "ScopedThread.hpp"

// Write a program where multiple threads increment a shared counter. Use a std::mutex to protect the shared counter and ensure its value is updated correctly.
// 	-Challenge: Remove the mutex and observe the effects. Can you explain why the counter might not increment correctly?
// 	-Extra Credit: Use std::atomic<int> instead of a mutex. How does this compare in performance and correctness?

struct counter{
    int counter_{0};
    std::mutex mx_;
};

void IncrementCounter(counter& c){
    std::stringstream ss{};

    for(int i = 0; i < 10; ++i){
        {
            std::lock_guard<std::mutex> lg{c.mx_};
            c.counter_++;
            ss << "Thread " << std::this_thread::get_id() << " set "
                    << "the counter to " << c.counter_ << "\n";
            std::cout << ss.str();
            ss.str("");
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int ex1(){
    counter c{};
    std::vector<ScopedThread> vec{};

    for(int i = 0; i < 10; ++i){
        vec.push_back(ScopedThread{
                      std::move(std::thread{IncrementCounter, std::ref(c)})
                      });
    }
    return 0;
}