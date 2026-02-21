#include <random>
#include <iostream>
#include <cstdint>

using namespace std;

// Overview:
// 概述：
// - This small module demonstrates properties of pseudorandom number generators (PRNG): determinism, period, seed reproducibility, and different distributions.
// - 本模块演示伪随机数生成器（PRNG）的特性：确定性、周期、种子可复现性以及不同概率分布的生成。

namespace rnd {
    // Engine type and global engine instance
    // 引擎类型与全局引擎实例
    using engine_t = std::mt19937;

    static engine_t eng{std::random_device{}()};

    // Seed the engine with a given value
    // 用指定的值设置种子
    inline void seed(uint32_t s)
    {
        eng.seed(s);
    }

    // Seed the engine using random_device and return the seed used
    // 使用 random_device 设置种子并返回所用种子
    inline uint32_t seed_from_random_device()
    {
        std::random_device rd;
        uint32_t s = static_cast<uint32_t>(rd());
        eng.seed(s);
        return s;
    }

    // Uniform integer in [a, b]
    // 整数均匀分布，区间 [a, b]
    inline int uniform_int(int a, int b)
    {
        std::uniform_int_distribution<int> d(a, b);
        return d(eng);
    }

    // Uniform real in [a, b)
    // 实数均匀分布，区间 [a, b)
    inline double uniform_real(double a = 0.0, double b = 1.0)
    {
        std::uniform_real_distribution<double> d(a, b);
        return d(eng);
    }

    // Normal (Gaussian) distribution
    // 正态（高斯）分布
    inline double normal(double mean = 0.0, double stddev = 1.0)
    {
        std::normal_distribution<double> d(mean, stddev);
        return d(eng);
    }

    // Bernoulli (true with probability p)
    // 伯努利分布（以概率 p 返回 true）
    inline bool bernoulli(double p = 0.5)
    {
        std::bernoulli_distribution d(p);
        return d(eng);
    }
}


int main()
{
    using namespace rnd;

    cout << "=== Random Number Generator Demo ===" << endl;
    cout << "=== 随机数生成器演示 ===" << endl;

    // Seed from random_device
    // 使用 random_device 设置种子
    uint32_t seed1 = seed_from_random_device();
    cout << "seed_from_random_device() = " << seed1 << "\n";

    cout << "uniform_int 1..10: ";
    for (int i = 0; i < 10; ++i)
    {
        cout << uniform_int(1, 10) << " ";
    }
    cout << "\n";

    cout << "uniform_real 0..1: ";
    for (int i = 0; i < 5; ++i)
    {
        cout << uniform_real() << " ";
    }
    cout << "\n";

    cout << "normal(0,1): ";
    for (int i = 0; i < 5; ++i)
    {
        cout << normal() << " ";
    }
    cout << "\n";

    cout << "bernoulli(0.3): ";
    for (int i = 0; i < 10; ++i)
    {
        cout << (bernoulli(0.3) ? "1" : "0");
    }
    cout << "\n";

    // Demonstrate reproducibility: seed with same value -> same sequence
    // 可复现性演示：使用相同种子 -> 相同序列
    seed(12345);
    cout << "seq A (seed 12345): ";
    for (int i = 0; i < 6; ++i)
    {
        cout << uniform_int(0, 100) << " ";
    }
    cout << "\n";

    seed(12345);
    cout << "seq B (seed 12345): ";
    for (int i = 0; i < 6; ++i)
    {
        cout << uniform_int(0, 100) << " ";
    }
    cout << "\n";

    return 0;
}