#pragma once
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <sstream>

class ScopedThread
{
private:
    std::thread t_{};
public:
    explicit ScopedThread(const std::thread t);
    ~ScopedThread();
    ScopedThread(ScopedThread& other) = delete;
    // move constructor
    ScopedThread(ScopedThread&& other) noexcept;
    // move assignment operator
    ScopedThread& operator=(ScopedThread&& other) noexcept;
};


int ex1();