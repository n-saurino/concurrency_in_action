#pragma once
#include <iostream>
#include <thread>

/*
Exercise 2: Thread Function Argument Passing

	1.	Passing Parameters: Write a function that takes an integer and 
    multiplies it by two. Create a thread that runs this function and passes an 
    integer as an argument.
	•	Challenge: Try passing additional arguments to the thread function by 
    capturing them in a lambda function.
	2.	Passing by Reference vs. Value: Modify the function to take an integer 
    reference as a parameter. Pass a variable from the main function and observe
     the behavior when you use std::ref() and without it.
	•	Discussion Question: Why is std::ref necessary when passing arguments by
     reference to threads? Thread constructors blindly copy all function 
     arguments. So you will try to call TimesTwo with an rvalue resulting in
     a compilation error because you tried to pass an rvalue to a function
     expecting a non-const reference.

*/

void TimesTwo(int& product){
    product *= 2;
}

int ex2(){
    int product{5};
    std::thread t{TimesTwo, std::ref(product)};
    t.join();
    std::cout << product << "\n";
    return 0;
}

