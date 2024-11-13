#pragma once
#include <iomanip>
#include <iostream>
#include <thread>
#include <string>

/*
Exercise 3: Functor and Lambda Thread Execution

	1.	Thread with Functor: Create a struct with a callable operator that takes
     a std::string and prints it. Initialize a thread with an instance of this 
     struct and observe the output.
	•	Follow-up: Compare this approach to using a regular function or lambda.
	2.	Thread with Lambda: Rewrite the same logic using a lambda function 
    instead of a functor. Compare the performance and readability with the 
    previous solution.
	•	Challenge: Experiment by capturing external variables by reference and 
    value in the lambda, and observe any differences.
*/

struct Example{
    void operator()(const std::string& input){
        std::stringstream ss;
        ss << input << "\n";
        std::cout << ss.str();
    }

};

void Print3(const std::string& input){
    std::stringstream ss;
    ss << input << "\n";
    std::cout << ss.str();
}

int ex3(){
    Example example{};
    std::string hello_world{"Hello, world!"};
    
    // create threads that take a string by reference and execute a Print function
    std::thread t_struct{example, std::ref(hello_world)};
    std::thread t_func{Print3, std::ref(hello_world)};
    std::thread t_lambda{[&](const std::string& input){
        std::stringstream ss;
        ss << input << "\n";
        std::cout << ss.str();
    }, std::ref(hello_world)};

    // join threads
    t_struct.join();
    t_func.join();
    t_lambda.join();

    return 0;
}