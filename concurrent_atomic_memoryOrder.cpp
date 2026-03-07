#include <iostream>
#include <thread>
#include <vector>
#include <atomic>

using namespace std;

// What this file does:
// 1) Demonstrates atomic increment correctness under high concurrency with memory_order_relaxed.
// 2) Demonstrates publish-subscribe visibility using memory_order_release / memory_order_acquire.
// 3) Highlights the key distinction: atomicity is not the same as ordering/visibility.
// 这个文件在做什么:
// 1) 演示在高并发下使用 memory_order_relaxed 的原子累加正确性。
// 2) 演示通过 memory_order_release / memory_order_acquire 建立发布-订阅可见性。
// 3) 强调核心区别: 原子性不等于顺序性/可见性。

// Typical scenarios explained by this file:
// 1) Global counters/statistics where only final numeric correctness is required.
// 2) One-time initialization handoff (producer publishes data, consumer waits for ready flag).
// 3) Lock-free fast paths that separate data write and readiness signaling.
// 4) Teaching and debugging memory-order bugs (stale read/reordering misunderstandings).
// 这个文件解释到的典型场景:
// 1) 全局计数器/统计指标场景, 只要求最终数值正确。
// 2) 一次性交接初始化场景 (生产者发布数据, 消费者等待 ready 信号)。
// 3) 无锁快速路径场景, 将“数据写入”与“就绪通知”分离。
// 4) 用于学习和排查内存序问题 (陈旧读/重排误解)。

// Goal: understand minimal practical usage of relaxed and acquire/release.
// 目标: 理解 relaxed 和 acquire/release 的最小实践。

void relaxed_counter_demo()
{
    // atomic guarantees fetch_add itself is atomic, so increments are not lost.
    // relaxed: guarantees atomicity only, without inter-thread ordering constraints.
    // atomic 保证 fetch_add 本身是原子的，不会丢增量。
    // relaxed: 只保证原子性，不提供线程间顺序约束。
    atomic<int> counter{0};
    vector<thread> threads;

    for (int i = 0; i < 8; ++i)
    {
        threads.emplace_back([&]()
                             {
                                for (int j = 0; j < 10000; ++j)
                                    // Multiple threads add 1 concurrently; final result is stably 80000.
                                    // We do not depend on visibility of other shared data here, so relaxed is sufficient.
                                    // 多线程并发加 1，最终结果稳定为 80000。
                                    // 这里不依赖其他共享数据的可见性，所以 relaxed 足够。
                                    counter.fetch_add(1, memory_order_relaxed); });
    }

    for (auto &t : threads)
        t.join();

    cout << "relaxed counter = " << counter.load(memory_order_relaxed) << " (expected 80000)\n";
}

void acquire_release_demo()
{
    // ready is the publish-subscribe signal flag, and data is the payload to publish.
    // ready 是“发布-订阅”信号位，data 是要被发布的数据。
    atomic<bool> ready{false};
    atomic<int> data{0};

    thread producer([&]()
                    {
                        // Write payload first. relaxed is used here because the real sync point is ready.
                        // 先写数据。这里用 relaxed，因为真正的同步点在 ready 上。
                        data.store(42, memory_order_relaxed);
                        // release: publishes the signal, ensuring prior writes (for example data=42)
                        // are not reordered after this store.
                        // release: 发布信号。它保证在这之前的写入（如 data=42）
                        // 不会被重排到这个 store 之后。
                        ready.store(true, memory_order_release); });

    thread consumer([&]()
                    {
                        // acquire: once it reads true, it synchronizes with producer's release.
                        // After synchronization, consumer must observe writes that happened before release.
                        // acquire: 一旦读到 true，与 producer 的 release 同步。
                        // 同步后，consumer 必须能看到 release 之前的写入结果。
                        while (!ready.load(memory_order_acquire))
                        {
                            this_thread::yield();
                        }
                        // Because acquire/release establishes happens-before,
                        // reading data with relaxed here can still observe producer's 42.
                        // 因为 acquire/release 已建立 happens-before，
                        // 这里即使用 relaxed 读取 data，也能看到 producer 写入的 42。
                        cout << "consumer sees data = " << data.load(memory_order_relaxed) << '\n'; });

    producer.join();
    consumer.join();
}

int main()
{
    // demo1: focuses on atomic counter correctness, not cross-variable visibility.
    // demo1: 关注“原子计数正确性”，不关注跨变量可见性。
    cout << "=== relaxed counter demo ===\n";
    relaxed_counter_demo();

    // demo2: focuses on cross-thread visibility and ordering, showing typical release/acquire usage.
    // demo2: 关注“跨线程可见性与顺序”，演示 release/acquire 典型用法。
    cout << "\n=== acquire/release demo ===\n";
    acquire_release_demo();

    cout << "\nNote: memory ordering controls visibility/reordering, not only atomicity.\n";
    cout << "注意：内存序不仅影响原子性，还影响可见性与重排约束。\n";
    return 0;
}
