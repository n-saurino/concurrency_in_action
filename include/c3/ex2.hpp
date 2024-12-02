#pragma once
#include <iostream>
#include <mutex>
#include <thread>

// Create a program that simulates a simple banking system with two accounts and
//  two threads transferring money between them.
// 	•	Implement the transfers using two std::mutex locks (one for each 
//       account).
// 	•	Use the std::lock function to avoid deadlocks.
// 	•	Challenge: Introduce an intentional deadlock by locking the mutexes in 
//       different orders. How can you detect and resolve the deadlock?

class ScopedThread{
public:
    explicit ScopedThread();
    ~ScopedThread();
    ScopedThread(ScopedThread& other) = delete;
    ScopedThread(ScopedThread&& other) noexcept;
    ScopedThread& operator=(ScopedThread&& other) noexcept;

private:
    std::thread t_{};
};

int ex2();