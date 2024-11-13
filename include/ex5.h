#include <iostream>
#include <thread>
#include <iomanip>

/*
Exercise 5: Thread Lifecycle and Resource Cleanup

	1.	Experiment with Thread Exit: Write a function that calculates the 
	factorial of a number, prints the result, and then terminates. Create a 
	thread that calls this function with a specific integer argument.
	•	Challenge: Add a mechanism to cancel the thread midway if it takes too 
	long (simulate a long task using sleep_for).
	
	2.	Resource Cleanup Verification: Monitor and verify that resources are 
	correctly released when a thread exits. Use std::this_thread::get_id() to 
	print thread IDs before and after execution, verifying each thread’s 
	lifecycle.
*/

class ScopedThread{
public:
	ScopedThread(std::thread t);
	~ScopedThread();
	
	// copy constructor
	ScopedThread(const ScopedThread& other) = delete;
	// assignment operator
	ScopedThread& operator=(const ScopedThread& other) = delete;
private:
	std::thread t_{};
};

ScopedThread::ScopedThread(std::thread t): t_{std::move(t)} {
	if(!t_.joinable()){
		throw std::logic_error("Thread object passd was not joinable or doesn't\
		 exist.");
	}
	std::stringstream ss;
	ss << "Thread " << std::this_thread::get_id() << " constructed!\n";
	std::cout << ss.str();
}

ScopedThread::~ScopedThread(){
	if(t_.joinable()){
		t_.join();
	}
	std::stringstream ss;
	ss << "Thread " << std::this_thread::get_id() << " exited!\n";
	std::cout << ss.str();
}

void Factorial(const int n, std::atomic<bool>& cancel){
	int fact{1};
	std::stringstream ss;
	for(int i = 2; i <= n; ++i){
		if(cancel.load()){
			ss << "Thread timed out!\n";
			std::cout << ss.str();
			return;
		}
		fact *= i;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	
	ss << fact << "\n";
	std::cout << ss.str();
}

int ex5(){
    std::atomic<bool> cancel{false};
	ScopedThread st1{std::move(std::thread{Factorial, 10, std::ref(cancel)})};
	
	std::this_thread::sleep_for(std::chrono::seconds(2));
	cancel.store(true);

	return 0;
}
