#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>
#include <future>
#include <type_traits>
#include <stdexcept>

using namespace std;

// What this file does:
// 1) Implements a minimal fixed-size thread pool using worker threads + a task queue.
// 2) Uses mutex + condition_variable to safely coordinate task production/consumption.
// 3) Demonstrates graceful shutdown: stop accepting work, wake workers, then join threads.
// 这个文件在做什么:
// 1) 实现一个最小固定大小线程池: 工作线程 + 任务队列。
// 2) 使用 mutex + condition_variable 安全协调任务生产与消费。
// 3) 演示优雅关闭流程: 停止标记、唤醒线程、最后 join 回收。

// Typical scenarios explained by this file:
// 1) Batch processing where many short tasks should be reused by a limited number of threads.
// 2) Backend job execution to avoid creating/destroying threads per task.
// 3) Learning core thread-pool mechanics before adding futures, priorities, or work stealing.
// 这个文件解释到的典型场景:
// 1) 批处理场景: 大量短任务复用少量线程执行。
// 2) 后端任务执行场景: 避免每个任务都创建/销毁线程。
// 3) 作为学习基础: 先掌握线程池核心机制, 再扩展 future/优先级/窃取调度。

class ThreadPool
{
private:
    vector<thread> workers;
    queue<function<void()>> tasks;
    mutex mtx;
    condition_variable cv;
    bool stopping = false;

public:
    explicit ThreadPool(size_t n)
    {
        // Start N worker threads. Each worker waits for tasks, executes one task,
        // and then returns to waiting state.
        // 启动 N 个工作线程。每个线程循环等待任务、执行任务、再继续等待。
        for (size_t i = 0; i < n; ++i)
        {
            workers.emplace_back([this, i]()
                                 {
                                    while (true)
                                    {
                                        function<void()> task;
                                        {
                                            unique_lock<mutex> lock(mtx);
                                            // Sleep until either:
                                            // 1) pool is stopping, or 2) task queue is not empty.
                                            // 休眠等待两个条件之一:
                                            // 1) 线程池进入停止状态, 或 2) 队列中有任务。
                                            cv.wait(lock, [this]() { return stopping || !tasks.empty(); });

                                            // Exit condition for workers:
                                            // if stopping is requested and no pending tasks remain.
                                            // 工作线程退出条件:
                                            // 已收到停止信号且队列已清空。
                                            if (stopping && tasks.empty())
                                                return;

                                            // Take one task from the queue under lock, then execute it
                                            // outside the critical section to reduce lock contention.
                                            // 在锁内取出一个任务, 然后在锁外执行, 降低锁竞争。
                                            task = move(tasks.front());
                                            tasks.pop();
                                        }
                                        task();
                                    } });
        }
    }

    void submit(function<void()> task)
    {
        {
            lock_guard<mutex> lock(mtx);
            if (stopping)
                throw runtime_error("submit on stopped ThreadPool");
            // Producer pushes a new task into the shared queue.
            // 生产者将新任务压入共享队列。
            tasks.push(move(task));
        }
        // Wake one sleeping worker to consume the new task.
        // 唤醒一个休眠中的工作线程来消费该任务。
        cv.notify_one();
    }

    template <class F, class... Args>
    auto submit(F &&f, Args &&...args) -> future<invoke_result_t<F, Args...>>
    {
        using ReturnT = invoke_result_t<F, Args...>;

        // Wrap callable + arguments into a packaged_task so we can obtain a future.
        // 把可调用对象和参数封装成 packaged_task, 以便返回 future。
        auto task_ptr = make_shared<packaged_task<ReturnT()>>(
            bind(forward<F>(f), forward<Args>(args)...));

        future<ReturnT> fut = task_ptr->get_future();
        submit([task_ptr]() { (*task_ptr)(); });
        return fut;
    }

    ~ThreadPool()
    {
        {
            lock_guard<mutex> lock(mtx);
            // Tell workers to stop after draining remaining tasks.
            // 通知工作线程: 把剩余任务做完后停止。
            stopping = true;
        }

        // Wake all workers so they can observe stopping flag and exit.
        // 唤醒所有工作线程, 让它们检查停止标记并退出。
        cv.notify_all();
        for (auto &w : workers)
        {
            if (w.joinable())
                w.join();
        }
    }
};

int main()
{
    cout << "=== thread pool demo ===\n";

    // Create a pool with 3 workers.
    // 创建一个包含 3 个工作线程的线程池。
    ThreadPool pool(3);

    // Keep one group of fire-and-forget tasks for basic scheduling demonstration.
    // 保留一组无返回值任务, 用于演示基础调度。
    atomic<int> done{0};

    for (int i = 1; i <= 8; ++i)
    {
        pool.submit([i, &done]()
                    {
                        // Simulate task cost differences.
                        // 模拟任务耗时差异。
                        this_thread::sleep_for(chrono::milliseconds(80 * i));
                        cout << "task " << i << " done by thread " << this_thread::get_id() << '\n';
                        done.fetch_add(1); });
    }

    // Busy-wait with short sleep for demo simplicity.
    // For production code, condition_variable/future is preferred for waiting.
    // 这里用短暂 sleep 轮询仅为演示简化。
    // 生产代码更推荐 condition_variable/future 等等待机制。
    while (done.load() < 8)
    {
        this_thread::sleep_for(chrono::milliseconds(50));
    }

    // Demonstrate submit that returns future<T>.
    // 演示带返回值的 submit, 返回 future<T>。
    vector<future<int>> futures;
    for (int x = 1; x <= 5; ++x)
    {
        futures.push_back(pool.submit([x]() {
            this_thread::sleep_for(chrono::milliseconds(40));
            return x * x;
        }));
    }

    cout << "squares from futures: ";
    for (size_t i = 0; i < futures.size(); ++i)
    {
        cout << futures[i].get();
        if (i + 1 < futures.size())
            cout << ", ";
    }
    cout << '\n';

    cout << "all tasks completed\n";
    return 0;
}
