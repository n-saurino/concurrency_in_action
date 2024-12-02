#pragma once
#include <thread>
#include <iostream>

class ScopedThread
{
public:
    explicit ScopedThread(std::thread t);
    ~ScopedThread();
    ScopedThread(ScopedThread& other) = delete;
    // move constructor
    ScopedThread(ScopedThread&& other) noexcept;
    // move assignment operator
    ScopedThread& operator=(ScopedThread&& other) noexcept;

private:
    std::thread t_{};
};