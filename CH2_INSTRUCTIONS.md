Chapter 2 of C++ Concurrency in Action focuses on managing threads, covering fundamental concepts like starting, joining, detaching threads, and some best practices in using std::thread. Here are some exercises to reinforce your understanding of this chapter.

Exercise 1: Simple Thread Creation and Management

	1.	Thread Creation: Write a function that takes a string and prints it with a prefix indicating the thread’s unique identifier. Create two threads to print different strings concurrently.
	•	Challenge: Ensure that each thread outputs its message uniquely and separately, with no overlap.
	2.	Joining Threads: Modify your function to add a delay (std::this_thread::sleep_for) in one of the threads. Join both threads in the main function and observe how join affects the program’s execution flow.
	•	Discussion Question: What happens if you remove the join() calls?
	3.	Detaching Threads: Detach one of the threads instead of joining it. Observe and record the program’s behavior.
	•	Follow-up: What potential problems could arise with detached threads in a larger application?

Exercise 2: Thread Function Argument Passing

	1.	Passing Parameters: Write a function that takes an integer and multiplies it by two. Create a thread that runs this function and passes an integer as an argument.
	•	Challenge: Try passing additional arguments to the thread function by capturing them in a lambda function.
	2.	Passing by Reference vs. Value: Modify the function to take an integer reference as a parameter. Pass a variable from the main function and observe the behavior when you use std::ref() and without it.
	•	Discussion Question: Why is std::ref necessary when passing arguments by reference to threads?

Exercise 4: RAII for Thread Management

	1.	Thread RAII Wrapper: Write a simple RAII class named ScopedThread that takes a std::thread in its constructor and joins it in its destructor. Use this class to manage a thread without explicitly calling join in the main function.
	•	Discussion Question: What are the advantages of using an RAII-style class for managing threads?
	2.	Experiment with Exception Handling: Modify your program to throw an exception in the middle of the threaded execution. Observe and note what happens to the threads when an exception occurs.
	•	Follow-up: How does RAII help manage resources in the presence of exceptions?

Exercise 5: Thread Lifecycle and Resource Cleanup

	1.	Experiment with Thread Exit: Write a function that calculates the factorial of a number, prints the result, and then terminates. Create a thread that calls this function with a specific integer argument.
	•	Challenge: Add a mechanism to cancel the thread midway if it takes too long (simulate a long task using sleep_for).
	2.	Resource Cleanup Verification: Monitor and verify that resources are correctly released when a thread exits. Use std::this_thread::get_id() to print thread IDs before and after execution, verifying each thread’s lifecycle.

Exercise 6: Exploring std::thread::hardware_concurrency

	1.	Detect Hardware Concurrency: Write a program that queries std::thread::hardware_concurrency to check the number of threads your hardware can optimally support. Print the result.
	•	Follow-up: Experiment by creating more threads than hardware_concurrency returns. How does this affect performance?
	2.	Thread Pool Simulation: Use hardware_concurrency as a basis to set up a thread pool of worker threads that each perform a basic task (e.g., print a message with its thread ID). After all threads finish, observe the effect of different thread pool sizes.

Exercise 7: Combining Threads with Mutexes

	1.	Safe Access to Shared Data: Initialize a global counter and create multiple threads that each increment the counter. Use a std::mutex to ensure safe access to the shared variable.
	•	Challenge: Remove the mutex and observe the effects on the program output.
	2.	Discussion: Explain why using a mutex is necessary in this case. How would this example be affected in a high-frequency trading environment?

