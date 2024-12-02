#pragma once
#include <iostream>
#include <mutex>
#include <thread>
#include <sstream>
#include <iomanip>
#include "Random.h"
#include "ScopedThread.hpp"


// Create a program that simulates a simple banking system with two accounts and
//  two threads transferring money between them.
// 	•	Implement the transfers using two std::mutex locks (one for each 
//       account).
// 	•	Use the std::lock function to avoid deadlocks.
// 	•	Challenge: Introduce an intentional deadlock by locking the mutexes in 
//       different orders. How can you detect and resolve the deadlock?

struct Account{
    int account_id_{0};
    double account_{100};
    std::mutex mx_{};
};

void Transfer(Account& a1, Account& a2){
    int transactions{Random::get(2,10)};
    std::stringstream ss{};
    for(int i = 0; i < transactions; ++i){
        {
        std::scoped_lock<std::mutex, std::mutex> sl(a1.mx_, a2.mx_);
        double transfer_amount{a1.account_*
                               static_cast<double>(Random::get(1,99))/100.0};
        a1.account_ -= transfer_amount;
        a2.account_ += transfer_amount;
        ss << std::fixed << std::setprecision(2) 
           << "Transferring " << transfer_amount << " from " << a1.account_id_
           << " to " << a2.account_id_ << " using thread " 
           << std::this_thread::get_id() << "\n";

        ss << "Account Balances:\n" 
           << "Account 1: " << a1.account_ << "\n"
           << "Account 2: " << a2.account_ << "\n";
        
        std::cout << ss.str();
        ss.str("");
        }
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
}

int ex2(){
    Account account_1{1};
    Account account_2{2};

    ScopedThread thread_1{std::move(std::thread{Transfer, std::ref(account_1), 
                          std::ref(account_2)})};
    ScopedThread thread_2{std::move(std::thread{Transfer, std::ref(account_2), 
                          std::ref(account_1)})};
    return 0;
}