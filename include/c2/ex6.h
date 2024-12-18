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
    // Define the move constructor
    ScopedThread(ScopedThread&& other) noexcept : t_(std::move(other.t_)) {}

    // Define the move assignment operator
    ScopedThread& operator=(ScopedThread&& other) noexcept {
        if (this != &other) {
            if (t_.joinable()) {
                t_.join();
            }
            t_ = std::move(other.t_);
        }
        return *this;
    }
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
    int AddTask(std::function<void()>&& task);
    void Run();
    void Wait();
private:
    std::vector<ScopedThread> threads_{};
    std::deque<std::function<void()>> tasks_{};
    std::mutex mx_{};
    std::condition_variable condition_{};
    std::condition_variable tasks_done_{};
    std::size_t active_tasks_{0};
    bool stop{false};
};

void ThreadPool::Wait(){
    std::unique_lock<std::mutex> ul{mx_};
    tasks_done_.wait(ul, [this](){return tasks_.empty() && active_tasks_ == 0;});
}

// function call that a thread is started with on Pool initialization
void ThreadPool::Run(){
    while(true){
        std::unique_lock<std::mutex> ul{mx_};
        condition_.wait(ul, [this]{return stop || !tasks_.empty();});
        
        // termination condition to end pool
        if(stop && tasks_.empty()){
            return;
        }
        // it's our turn to get a task!f
        std::function<void()> task = std::move(tasks_.front());
        tasks_.pop_front();
        // increment active tasks
        ++active_tasks_;
        ul.unlock();
        // Execute the task outside the critical section
        if (task) {
            task();
        }

        {
            std::lock_guard<std::mutex> lg{mx_};
            --active_tasks_;
            if(tasks_.empty() && active_tasks_ == 0){
                tasks_done_.notify_one();
            }
        }
    }
}

// ThreadPool constructor will take a total number of threads to start
ThreadPool::ThreadPool(size_t num_threads){
    // need to create threads and add them to the threads vector 
    for(int i = 0; i < num_threads; ++i){
        // ScopedThread constructor will call Run() automatically
        std::thread t([this](){Run();});
        // must transfer ownership of thread with move!
        threads_.push_back(ScopedThread{std::move(t)});
    }
}

ThreadPool::~ThreadPool(){
    // acquire lock
    std::unique_lock<std::mutex> ul{mx_};
    // update stop
    stop = true;
    ul.unlock();
    // notify all threads
    condition_.notify_all();

    // force destructor of Scoped threads by clearing the threads vector
    threads_.clear();
}

int ThreadPool::AddTask(std::function<void()>&& task){
    // lock the queue
    {
        std::lock_guard<std::mutex> lg(mx_);
        tasks_.emplace_back(std::move(task));
    }
    // signal a thread in the pool
    condition_.notify_one();

    return 0;
}

void GetWords(const std::string& line, int& word_count){
    std::stringstream ss(line);
    std::string one_word;
 
    word_count += std::distance(std::istream_iterator<std::string>(ss), 
                               std::istream_iterator<std::string>());
}

void TestFunc(int task_no){
    std::stringstream ss;
    ss << "Executing task " << task_no << " on thread " 
              << std::this_thread::get_id() << "!\n";
    std::cout << ss.str();
    std::this_thread::sleep_for(std::chrono::seconds(1));
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
    // set the number of threads based on hardware
    size_t cpus = std::thread::hardware_concurrency() - 1;

    ThreadPool pool{cpus};

    std::ifstream file;
    file.open("test.txt", std::ifstream::in);

    std::string input{};
    int result{0};
    std::mutex result_mutex;
    auto start = std::chrono::high_resolution_clock::now();

    if(file.is_open()){
        while(getline(file, input)){    
            pool.AddTask([input, &result, &result_mutex](){
                int local_count{0};
                GetWords(input, local_count);
                std::lock_guard<std::mutex> lg{result_mutex};
                result += local_count;
                });
        }
    }

    // wait for task completion so that we can get accurate time
    pool.Wait();

    auto end = std::chrono::high_resolution_clock::now();

    auto total_time = std::chrono::duration_cast<std::chrono::microseconds>
                      (end - start);

    std::cout << "There are " << result << " words in the corpus!\n";
    std::cout << "Total runtime: " << total_time.count() << "\n";
    return 0;
}