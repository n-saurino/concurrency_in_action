#include <iomanip>
#include <iostream>
#include <thread>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <deque>
#include <mutex>

/*
Exercise 6: Exploring std::thread::hardware_concurrency

	1.	Detect Hardware Concurrency: Write a program that queries 
    std::thread::hardware_concurrency to check the number of threads your 
    hardware can optimally support. Print the result.
	•	Follow-up: Experiment by creating more threads than hardware_concurrency
     returns. How does this affect performance?
	2.	Thread Pool Simulation: Use hardware_concurrency as a basis to set up a 
    thread pool of worker threads that each perform a basic task (e.g., print a 
    message with its thread ID). After all threads finish, observe the effect of
     different thread pool sizes.

Exercise 7: Combining Threads with Mutexes

	1.	Safe Access to Shared Data: Initialize a global counter and create 
    multiple threads that each increment the counter. Use a std::mutex to ensure
     safe access to the shared variable.
	•	Challenge: Remove the mutex and observe the effects on the program 
    output.
	2.	Discussion: Explain why using a mutex is necessary in this case. How 
    would this example be affected in a high-frequency trading environment?
*/

class ScopedThread{
public:
    ScopedThread();
    ~ScopedThread();
    ScopedThread(const ScopedThread& other) = delete;
    ScopedThread& operator=(const ScopedThread& other) = delete;

private:
    std::thread t_;
    void Run();
};

ScopedThread::ScopedThread(){
    if(!t_.joinable()){
        throw std::logic_error("Couldn't create thread!");
    }
    t_{Run};
}

ScopedThread::~ScopedThread(){
    if(t_.joinable()){
        t_.join();
    }
}

void ScopedThread::Run(){
    // sleep until signaled to wake up

    // then wake up and remove a task from the queue
    std::string task = ThreadPool::tasks_.front();
    int word_count{};
    GetWords(task, word_count);
}

class ThreadPool{
public:
    // constructors
    ThreadPool(size_t total_threads);
    ~ThreadPool();
    ThreadPool(const ThreadPool& other) = delete;
    ThreadPool& operator=(const ThreadPool& other) = delete;
    
    // user-defined functions
    // returns 1 if error, returns 0 if success
    int AddTask(const std::string& task);
    friend class ScopedThread;

private:
    std::vector<ScopedThread> threads_{};
    std::deque<std::string> tasks_{};
    std::mutex mx_{};
};

// ThreadPool constructor will take a total number of threads to start
ThreadPool::ThreadPool(size_t total_threads): threads_{total_threads}{

}

ThreadPool::~ThreadPool(){

}

int ThreadPool::AddTask(const std::string& line){
    tasks_.push_back(line);
    // signal a thread in the pool

}

void GetWords(const std::string& line, int& word_count){
    std::stringstream ss(line);
    std::string one_word;
 
    word_count += std::distance(std::istream_iterator<std::string>(ss), 
                               std::istream_iterator<std::string>());
}

/*
Write a program that takes in a file, uses a thread pool to count the words
in the file and outputs the total character count of the file

We will assign lines as tasks to a task_queue that will be polled by the thread
pool

The thread pool will have its size set based on 
std::thread::hardware_concurrency - 1

The threads in the thread pool will always be running, waiting for the task
queue to have a new task to process

Each thread will be instantiated with a function Run() that runs the thread
in a tight loop and polls the task queue

Threads will need to lock a mutex in order to access the queue

Consider creating a LockGuard class that manages the mutex acquisition/release
*/

int ex6(){

    std::ifstream file;
    file.open("corpus.txt", std::ifstream::in);

    std::string input{};
    int result{0};

    auto start = std::chrono::high_resolution_clock::now();
    if(file.is_open()){
        while(getline(file, input)){    
            GetWords(input, result);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto total_time = std::chrono::duration_cast<std::chrono::microseconds>
                      (end - start);

    std::cout << "There are " << result << " words in the corpus!\n";
    std::cout << "Total runtime: " << total_time.count() << "\n";
    
    size_t cpus = std::thread::hardware_concurrency();
    std::cout << "CPUs: " << cpus << "\n";
    return 0;
}