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
#include <condition_variable>

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
    ScopedThread(std::thread t);
    ~ScopedThread();
    ScopedThread(const ScopedThread& other) = delete;
    ScopedThread& operator=(const ScopedThread& other) = delete;

private:
    std::thread t_;
};

ScopedThread::ScopedThread(std::thread t): t_{std::move(t)}{
    if(!t_.joinable()){
        throw std::logic_error("No thread!");
    }
}

ScopedThread::~ScopedThread(){
    if(t_.joinable()){
        t_.join();
    }
}

class ThreadPool{
public:
    // constructors
    ThreadPool(size_t num_threads);
    ~ThreadPool();
    ThreadPool(const ThreadPool& other) = delete;
    ThreadPool& operator=(const ThreadPool& other) = delete;
    
    // user-defined functions
    // returns 1 if error, returns 0 if success
    int AddTask(std::function<void>&& task);
    void Run();
private:
    std::vector<ScopedThread> threads_{};
    std::deque<std::function<void()>> tasks_{};
    std::mutex mx_{};
    std::condition_variable condition{};
    bool stop{false};
};

void ThreadPool::Run(){
    while(true){
        std::unique_lock<std::mutex> ul{mx_};
        condition.wait(ul, [this]{return stop || !tasks_.empty();});
        
        // termination condition to end pool
        if(stop && tasks_.empty()){
            return;
        }
        // it's our turn to get a task!
        ul.lock();
        std::function<void()> task = std::move(tasks_.front());
        tasks_.pop_front();
        ul.unlock();
        task();
    }
}

// ThreadPool constructor will take a total number of threads to start
ThreadPool::ThreadPool(size_t num_threads): threads_(num_threads){
    // need to create threads and add them to the threads vector 
    for(int i = 0; i < num_threads; ++i){
        // ScopedThread constructor will call Run() automatically
        std::thread t{ThreadPool::Run};
        // must transfer ownership of thread with move!
        threads_.push_back(ScopedThread{std::move(t)});
    }
}

ThreadPool::~ThreadPool(){

}

int ThreadPool::AddTask(std::function<void>&& task){
    // lock the queue
    std::lock_guard lg(mx_);
    tasks_.emplace_back(task);

    // signal a thread in the pool
    condition.notify_one();
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

    /*
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
    
    std::cout << "CPUs: " << cpus << "\n";
    */

   size_t cpus = std::thread::hardware_concurrency();

    ThreadPool pool{cpus};

    for(int i = 0; i < 100; ++i){
        pool.AddTask([](int task_no){
            std::cout << "Executing task " << task_no << " on thread " 
                      << std::this_thread::get_id() << "!\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
        });
    }

    return 0;
}