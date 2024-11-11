#pragma once
#include <iostream>
#include <thread>
#include <iomanip>
/*
Exercise 4: RAII for Thread Management

	1.	Thread RAII Wrapper: Write a simple RAII class named ScopedThread that 
	takes a std::thread in its constructor and joins it in its destructor. Use 
	this class to manage a thread without explicitly calling join in the main 
	function.
	•	Discussion Question: What are the advantages of using an RAII-style 
	class for managing threads? It is a cleaner way for managing memory:
	Resourse acquisition is initialization makes it so you don't forget
	to deallocate memory or wait on a thread
	2.	Experiment with Exception Handling: Modify your program to throw an 
	exception in the middle of the threaded execution. Observe and note what 
	happens to the threads when an exception occurs.
	•	Follow-up: How does RAII help manage resources in the presence of 
	exceptions?
*/

class ScopedThread{
public:
	std::thread t_{};
	explicit ScopedThread(std::thread t);
	~ScopedThread();

private:

};

ScopedThread::ScopedThread(std::thread t): t_{std::move(t)}{
	if(!t_.joinable())
            throw std::logic_error("No thread");
}

ScopedThread::~ScopedThread(){
	if(t_.joinable()){
		t_.join();
	}
}

int ex4(){
	std::string hello_world{"Hello, world!"};
	std::thread t{[&](const std::string& input){
		try
		{
			std::stringstream ss;
			ss << input << "\n";
			std::cout << ss.str();
			throw std::logic_error("Forcing a thrown exception");
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
		
		
	}, std::ref(hello_world)};

	ScopedThread st{std::move(t) };

	return 0;
}
