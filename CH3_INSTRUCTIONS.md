Below are end-of-chapter exercises tailored for Chapter 3 of “C++ Concurrency in Action”, which focuses on sharing data between threads and ensuring data consistency. These exercises will help solidify the concepts covered in the chapter.

1. Safe Data Sharing with Mutex

Write a program where multiple threads increment a shared counter. Use a std::mutex to protect the shared counter and ensure its value is updated correctly.
	•	Challenge: Remove the mutex and observe the effects. Can you explain why the counter might not increment correctly?
	•	Extra Credit: Use std::atomic<int> instead of a mutex. How does this compare in performance and correctness?

2. Deadlock Prevention

Create a program that simulates a simple banking system with two accounts and two threads transferring money between them.
	•	Implement the transfers using two std::mutex locks (one for each account).
	•	Use the std::lock function to avoid deadlocks.
	•	Challenge: Introduce an intentional deadlock by locking the mutexes in different orders. How can you detect and resolve the deadlock?

3. Shared State with std::future and std::promise

Implement a program where one thread calculates the factorial of a number and shares the result with the main thread using std::promise and std::future.
	•	Hint: Use std::promise to set the value of the factorial in the worker thread and retrieve it in the main thread using std::future.

4. Thread-Safe Lazy Initialization

Create a singleton class that performs thread-safe lazy initialization using std::call_once and std::once_flag.
	•	Implement a static function get_instance() that ensures the singleton is initialized only once, even if multiple threads try to access it concurrently.
	•	Challenge: Compare the performance of std::call_once with a double-checked locking implementation.

5. Producer-Consumer Problem

Simulate a producer-consumer scenario where:
	•	A producer thread generates random integers and adds them to a shared queue.
	•	A consumer thread retrieves integers from the queue and processes them.

Use std::mutex and std::condition_variable to ensure proper synchronization between the producer and consumer.
	•	Hint: The producer should notify the consumer when a new item is added, and the consumer should wait for the producer to add items if the queue is empty.

6. Read-Heavy Workload with std::shared_mutex

Simulate a read-heavy application, such as a cache, where:
	•	Multiple reader threads access a shared resource.
	•	One writer thread updates the resource occasionally.

Use std::shared_mutex to allow multiple readers or one writer at a time.
	•	Challenge: Compare the performance of std::shared_mutex with std::mutex for this scenario.

7. Parallel Matrix Multiplication

Write a program to perform matrix multiplication using multiple threads. Each thread should compute a subset of the result matrix.
	•	Use std::vector to store the matrices and synchronize access to shared data with std::mutex if necessary.
	•	Hint: Divide the rows of the result matrix among the threads.
	•	Extra Credit: Optimize the implementation to avoid unnecessary synchronization.

8. Debugging Race Conditions

Write a program with an intentional race condition where multiple threads update a shared std::vector without synchronization.
	•	Use a thread sanitizer tool to identify the race condition.
	•	Add proper synchronization (e.g., std::mutex or std::atomic) to fix the issue.
	•	Challenge: Modify the program to intentionally introduce a deadlock and debug it.

9. Thread Lifetime Management

Implement a thread pool class where:
	•	Threads are created during initialization and continuously wait for tasks to execute.
	•	Tasks are added to a queue and assigned to threads dynamically.

Use std::condition_variable to notify worker threads when new tasks are available.

10. Performance Comparison

Write a program to measure the performance of various synchronization primitives:
	•	Compare std::mutex, std::shared_mutex, and std::atomic for a workload involving both reads and writes.
	•	Hint: Use a shared counter where 90% of the operations are reads and 10% are writes.

These exercises will challenge readers to apply the concepts from Chapter 3, such as thread safety, avoiding data races, and ensuring efficient synchronization. Let me know if you’d like solutions or further guidance on any of these!