#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <semaphore> // Requires C++20 (需要 C++20 标准)
#include <future>    // For promise, future, async

using namespace std;

// Helper to print separator
void printSeparator(const string &title)
{
    cout << "\n========================================\n";
    cout << "   " << title << "\n";
    cout << "========================================\n";
}

// ============================================================
// Demo 1: Binary Semaphore (二元信号量)
// Scenario: A worker waits for a signal from the main thread.
// 场景：工人线程等待主线程发出的信号。
// ============================================================
void demoBinarySemaphore()
{
    printSeparator("Demo 1: Binary Semaphore");

    // 0 means locked initially.
    // 0 表示初始状态是锁定的（无信号）。
    std::binary_semaphore sem(0);

    // Worker thread using Lambda
    // 使用 Lambda 的工作线程
    thread worker([&]()
                  {
                      cout << "[Worker] Waiting for signal... (sem.acquire)" << endl;
                      // cout << "[工人] 等待信号... (sem.acquire)" << endl;

                      // Block until count > 0
                      // 阻塞直到计数 > 0
                      sem.acquire();

                      cout << "[Worker] Signal received! Working..." << endl;
                      // cout << "[工人] 收到信号！开始工作..." << endl;
                  });

    cout << "[Main] Sleeping for 2 seconds..." << endl;
    this_thread::sleep_for(chrono::seconds(2));

    cout << "[Main] Releasing signal... (sem.release)" << endl;
    // cout << "[主线程] 释放信号... (sem.release)" << endl;

    // Increment counter to 1
    // 计数变为1，唤醒工人
    sem.release();

    worker.join();
}

// ============================================================
// Demo 2: Counting Semaphore (计数信号量)
// Scenario: 10 threads compete for 3 slots.
// 场景：10个线程争抢3个位置（许可证）。
// ============================================================
void demoCountingSemaphore()
{
    printSeparator("Demo 2: Counting Semaphore");

    // Max count is specific (e.g. 10), init count is 0 (all blocked)
    // 最大计数指定（例如10），初始计数为0（全部阻塞）
    std::counting_semaphore<10> sem(0);
    vector<thread> threads;

    // Create 6 workers
    // 创建 6 个工人
    for (int i = 0; i < 6; ++i)
    {
        threads.emplace_back([&, i]()
                             {
            cout << "[Worker " << i << "] Waiting for slot..." << endl;
            
            sem.acquire(); // Decrease counter, wait if 0 | 计数减1，为0则等待
            
            cout << "[Worker " << i << "] Got slot! Running..." << endl;
            // Simulate work | 模拟工作
            this_thread::sleep_for(chrono::milliseconds(500)); });
    }

    this_thread::sleep_for(chrono::seconds(1));

    cout << "\n[Main] Releasing 3 slots (permits)..." << endl;
    // cout << "[主线程] 释放 3 个名额（许可证）..." << endl;
    sem.release(3); // 3 workers will wake up | 3个工人会醒来

    this_thread::sleep_for(chrono::seconds(2));

    cout << "\n[Main] Releasing remaining 3 slots to finish..." << endl;
    // cout << "[主线程] 释放剩余 3 个名额以结束程序..." << endl;
    // Important: We must release others, or join() will hang forever!
    // 重要：必须释放剩下的，否则 join() 会永远卡住！
    sem.release(3);

    for (auto &t : threads)
    {
        t.join();
    }
}

// ============================================================
// Demo 3: Promise & Future (承诺与期值)
// Scenario: Main thread promises to set a value, worker waits for it.
// 场景：主线程承诺设置一个值，子线程等待这个值。
// ============================================================
void demoPromiseFuture()
{
    printSeparator("Demo 3: Promise & Future");

    // 1. Create Promise (The Input Side)
    // 1. 创建 Promise（入口端）
    std::promise<int> prom;

    // 2. Get Future (The Output Side)
    // 2. 获取 Future（出口端）
    std::future<int> fut = prom.get_future();

    // 3. Pass future to thread (Must use std::move or reference because future is non-copyable)
    // 3. 把 future 传给线程（必须用 move 或引用，因为 future 不能拷贝）
    thread consumer([](std::future<int> f)
                    {
        cout << "[Consumer] Waiting for value... (fut.get)" << endl;
        
        // Block until value is set
        // 阻塞直到值被设置
        int result = f.get(); 
        
        cout << "[Consumer] Got value: " << result << endl; }, std::move(fut));

    cout << "[Producer] Calculating..." << endl;
    this_thread::sleep_for(chrono::seconds(2));

    cout << "[Producer] Setting value to 100..." << endl;
    // 4. Fulfill promise
    // 4. 兑现承诺
    prom.set_value(100);

    consumer.join();
}

// ============================================================
// Demo 4: Async & Task Chain (异步与任务链)
// Scenario: Task B starts only after Task A finishes, using dependencies.
// 场景：任务 B 依赖任务 A 的结果，A 完成后 B 才开始。
// ============================================================
void demoAsyncChain()
{
    printSeparator("Demo 4: Async & Task Chain");

    cout << "[Main] Starting Task A asynchronously..." << endl;

    // Step 1: Start Task A
    // std::async returns a future automatically
    // 第一步：启动任务 A。std::async 自动返回一个 future
    std::future<int> futureA = std::async(std::launch::async, []()
                                          {
        this_thread::sleep_for(chrono::seconds(1));
        cout << "  -> [Task A] Finished. Result: 10" << endl;
        return 10; });

    // Step 2: Start Task B (Depends on A)
    // We move futureA into Task B's lambda
    // 第二步：启动任务 B（依赖 A）。我们将 futureA 移动进 B 的 Lambda 中
    std::future<int> futureB = std::async(std::launch::async, [futA = std::move(futureA)]() mutable
                                          {
        cout << "  -> [Task B] Waiting for A..." << endl;
        
        // Wait for A's result
        // 等待 A 的结果
        int valA = futA.get(); 
        
        int valB = valA * 5; // Logic: result of A * 5
        cout << "  -> [Task B] Processed A's result. New Result: " << valB << endl;
        return valB; });

    // Step 3: Main thread waits for final result
    // 第三步：主线程等待最终结果
    cout << "[Main] Waiting for Task B chain to complete..." << endl;
    int finalResult = futureB.get();

    cout << "[Main] Final Chain Result: " << finalResult << endl;
}

int main()
{
    // Call demos one by one
    // 依次调用示例

    demoBinarySemaphore();

    demoCountingSemaphore();

    demoPromiseFuture();

    demoAsyncChain();

    printSeparator("All Demos Finished");
    return 0;
}