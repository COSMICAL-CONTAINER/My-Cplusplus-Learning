#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <future>
using namespace std;

// concurrency programming demo
// 并发编程示例

// Thread-safe data structure
// 线程安全的数据结构
class ThreadSafeQueue
{
private:
    queue<int> data;
    mutable mutex mtx;
    condition_variable cv;

public:
    void push(int value)
    {
        // Push value into queue
        // 入队操作
        lock_guard<mutex> lock(mtx);
        data.push(value);
        cv.notify_one();
    }

    bool pop(int &value)
    {
        // Pop value from queue
        // 出队操作
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [this]
                { return !data.empty(); });
        value = data.front();
        data.pop();
        return true;
    }

    bool empty() const
    {
        // Check if queue is empty
        // 检查队列是否为空
        lock_guard<mutex> lock(mtx);
        return data.empty();
    }

    size_t size() const
    {
        // Get queue size
        // 获取队列大小
        lock_guard<mutex> lock(mtx);
        return data.size();
    }
};

// Producer-consumer pattern
// 生产者-消费者模式
void producer(ThreadSafeQueue &queue, int id, int count)
{
    for (int i = 0; i < count; ++i)
    {
        int value = id * 100 + i;
        queue.push(value);
        // Producer produces value
        // 生产者生产数据
        cout << "Producer " << id << " produces: " << value << endl;
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}
// Consumer thread function with exit signal
// 消费者线程函数，支持退出信号
void consumer(ThreadSafeQueue &queue, int id)
{
    while (true)
    {
        int value;
        if (queue.pop(value))
        {
            // If value is special exit signal, break
            // 如果收到特殊退出信号，退出循环
            if (value == -1)
                break;
            cout << "Consumer " << id << " consumes: " << value << endl;
            // Consumer consumes value
            // 消费者消费数据
            this_thread::sleep_for(chrono::milliseconds(200));
        }
    }
}

int main()
{
    cout << "=== concurrent programming Demo ===" << endl;
    cout << "=== 并发编程示例 ===" << endl;

    // 1. 基本线程操作
    // 1. Basic thread operations
    {
        cout << endl;
        cout << "--- Basic thread operations ---" << endl;
        cout << "--- 基本线程操作 ---" << endl;
        auto start = chrono::high_resolution_clock::now();

        thread t1([]()
                  {
            cout << "Thread 1 started" << endl;
            this_thread::sleep_for(chrono::milliseconds(100));
            cout << "Thread 1 finished" << endl; });

        thread t2([]()
                  {
            cout << "Thread 2 started" << endl;
            this_thread::sleep_for(chrono::milliseconds(50));
            cout << "Thread 2 finished" << endl; });

        t1.join();
        t2.join();

        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end -
                                                                    start);
        cout << "Total time: " << duration.count() << "ms" << endl;
    }

    // 2. mutex and condition_variable
    // 2. 互斥锁和条件变量
    {
        cout << endl;
        cout << "--- mutex and condition_variable ---" << endl;
        cout << "--- 互斥锁和条件变量 ---" << endl;
        ThreadSafeQueue queue;

        // Start producer threads
        // 启动生产者线程
        thread producer1(producer, ref(queue), 1, 5);
        thread producer2(producer, ref(queue), 2, 5);

        // Start consumer threads
        // 启动消费者线程
        thread consumer1(consumer, ref(queue), 1);
        thread consumer2(consumer, ref(queue), 2);

        // Wait for producers to finish
        // add join() for producers to ensure they finish before sending exit signals
        // 等待生产者完成
        // 手动 join() 等待线程结束，否则主线程可能提前退出。
        producer1.join();
        producer2.join();

        // Let consumers run for a while then exit
        // 让消费者运行一段时间后退出
        this_thread::sleep_for(chrono::seconds(2));

        // Send exit signal to consumers
        // 向消费者线程发送退出信号
        queue.push(-1); // Consumer 1 exit
        queue.push(-1); // Consumer 2 exit

        consumer1.join();
        consumer2.join();
    }

    // 3. async and future
    // 3. async和future
    {
        cout << endl;
        cout << "--- async and future ---" << endl;
        cout << "--- 异步执行和期望值 ---" << endl;

        // Execute task asynchronously
        // 异步执行任务
        // std::async starts the task immediately (in a new thread or deferred, depending on policy).
        // std::thread starts the thread immediately, but you must call join() to wait for it to finish.
        // std::async 创建 future 时任务就会自动开始执行（可能新线程，也可能延迟，取决于策略）。
        // std::thread 创建时线程立即开始执行，但需要调用 join() 等待其结束。
        auto future1 = async(launch::async, []()
                             {
            this_thread::sleep_for(chrono::milliseconds(100));
            return 42; });

        auto future2 = async(launch::async, []()
                             {
            this_thread::sleep_for(chrono::milliseconds(50));
            return string("Hello from async"); });

        // Get result
        // 获取结果
        // future1.get() blocks the current thread until the asynchronous task finishes and returns the result.
        // future1.get() 会阻塞当前线程，直到异步任务完成并返回结果。
        cout << "future1 result: " << future1.get() << endl;
        cout << "future2 result: " << future2.get() << endl;

        // Use packaged_task
        // 使用packaged_task
        // std::packaged_task is a wrapper for a callable (function, lambda, etc.) that allows its result to be retrieved via a future.
        // std::packaged_task<int(int)> means a task that takes an int as input and returns an int as output.
        // can put fuction, lambda, etc. into packaged_task and get result through future.
        // std::packaged_task 是一个可调用对象的包装器，可以通过 future 获取其结果。
        // std::packaged_task<int(int)> 表示这个任务接受一个 int 参数，返回一个 int 结果。
        // 可以把函数、lambda 等包装进 packaged_task，然后用 future 获取其结果。
        packaged_task<int(int)> task([](int x)
                                     { return x * x; });
        auto future3 = task.get_future();

        // Directly execute the task in the current thread
        // 直接在当前线程执行任务
        // task(10);
        // cout << "packaged_task result: " << future3.get() << endl;

        // Or execute the task in a new thread
        // 或者在新线程中执行任务
        thread taskThread(move(task), 10);
        taskThread.join();

        cout << "packaged_task result: " << future3.get() << endl;
    }

    // 4. Atomic operations
    // 4. 原子操作
    {
        cout << endl;
        cout << "--- Atomic operations ---" << endl;
        cout << "--- 原子操作 ---" << endl;
        atomic<int> counter(0);
        // Atomic counter
        // 原子计数器

        vector<thread> threads;
        for (int i = 0; i < 10; ++i) 
        {
            threads.emplace_back([&counter]() 
            {
                for (int j = 0; j < 1000; ++j) 
                {
                    counter.fetch_add(1);
                }
            });
        }

        for (auto &t : threads)
        {
            t.join();
        }

        cout << "Final counter value: " << counter.load() << endl;
    }

    return 0;
}