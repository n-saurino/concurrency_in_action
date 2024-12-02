#pragma once
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <sstream>
#include "ScopedThread.hpp"

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