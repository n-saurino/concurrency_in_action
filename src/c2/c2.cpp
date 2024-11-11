#include <iostream>
#include <thread>
#include <iomanip>

/*
Exercise 1: Simple Thread Creation and Management

	1.	Thread Creation: Write a function that takes a string and prints it with
	 a prefix indicating the thread’s unique identifier. Create two threads to 
	 print different strings concurrently.
	•	Challenge: Ensure that each thread outputs its message uniquely and 
	separately, with no overlap.
	2.	Joining Threads: Modify your function to add a delay 
	(std::this_thread::sleep_for) in one of the threads. Join both threads in 
	the main function and observe how join affects the program’s execution flow.
	•	Discussion Question: What happens if you remove the join() calls?
	3.	Detaching Threads: Detach one of the threads instead of joining it. 
	Observe and record the program’s behavior.
	•	Follow-up: What potential problems could arise with detached threads in 
	a larger application?
*/

void Print(std::string_view input){
	std::stringstream ss;
	ss << std::this_thread::get_id() << ": " << input << "\n";
	std::cout << ss.str();
}

int main(int argc, char* argv[]){
    std::thread t1{Print, "child 1 thread!"};
	std::thread t2{Print, "child 2 thread!"};
	// std::this_thread::sleep_for(std::chrono::seconds(3));
	t1.detach();
	t2.join();
	Print("Parent thread!");
    return 0;
}
