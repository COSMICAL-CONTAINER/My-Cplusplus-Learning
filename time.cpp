#include <chrono>
#include <iostream>
#include <thread>
#include <ctime>
#include <iomanip>

using namespace std;

// This file consolidates time-related learning examples into one place.
// 包含与时间相关的学习示例：既有 C++ 的 <chrono>，也有 C 的 <time.h>（在 C++ 中为 <ctime>）用法。

// Helper: simulate CPU work
// 模拟 CPU 工作
static void busy_work(int iterations)
{
    volatile int x = 0;
    for (int i = 0; i < iterations; ++i)
        x += i;
}

// Chrono examples
// Chrono 示例
// Simple time helpers (moved here so we use only system headers, no custom time.h)
// 简易时间辅助函数（直接放在此处，使用系统头，不再依赖自定义 time.h）
namespace timeutil {
    using highres_clock = std::chrono::high_resolution_clock;

    // Return high-resolution time_point
    inline highres_clock::time_point now_highres()
    {
        return highres_clock::now();
    }

    // Return milliseconds since epoch (approx)
    inline long long now_ms()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(highres_clock::now().time_since_epoch()).count();
    }

    // Format milliseconds into human-readable H/M/S/ms string
    inline std::string format_ms(long long ms)
    {
        using namespace std::chrono;
        auto d = milliseconds(ms);
        auto hrs = duration_cast<hours>(d);
        d -= hrs;
        auto mins = duration_cast<minutes>(d);
        d -= mins;
        auto secs = duration_cast<seconds>(d);
        d -= secs;
        auto msecs = duration_cast<milliseconds>(d);

        char buf[128];
        if (hrs.count() > 0)
            snprintf(buf, sizeof(buf), "%lldh %lldm %llds %lldms",
                     (long long)hrs.count(), (long long)mins.count(), (long long)secs.count(), (long long)msecs.count());
        else if (mins.count() > 0)
            snprintf(buf, sizeof(buf), "%lldm %llds %lldms",
                     (long long)mins.count(), (long long)secs.count(), (long long)msecs.count());
        else
            snprintf(buf, sizeof(buf), "%llds %lldms",
                     (long long)secs.count(), (long long)msecs.count());
        return std::string(buf);
    }
}

static void demo_chrono()
{
    using namespace std::chrono_literals;

    cout << "=== Chrono Learning Demo ===\n";

    // high_resolution_clock: typically the clock with the smallest tick period available.
    // Note: depending on the standard library implementation, high_resolution_clock may be an alias
    // to either steady_clock or system_clock. Prefer steady_clock when you need a monotonic clock.
    // high_resolution_clock：通常是可用的分辨率最高的时钟。
    // 注意：在不同实现中，high_resolution_clock 可能是 steady_clock 或 system_clock 的别名；
    // 当需要单调性时请优先使用 steady_clock。
    auto t0 = timeutil::now_highres();
    busy_work(1000000);
    auto t1 = timeutil::now_highres();

    auto elapsed_ns = chrono::duration_cast<chrono::nanoseconds>(t1 - t0).count();
    auto elapsed_ms = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
    cout << "Elapsed: " << elapsed_ms << " ms (" << elapsed_ns << " ns)\n";
    cout << "Formatted: " << timeutil::format_ms(elapsed_ms) << "\n";

    // steady_clock is monotonic and not affected by system time changes (e.g., NTP adjustments).
    // Use steady_clock to measure intervals that must not go backward.
    // steady_clock 是单调的，不受系统时间调整（如 NTP）影响。
    // 使用 steady_clock 测量不应出现回退的时间间隔。
    auto s0 = chrono::steady_clock::now();
    std::this_thread::sleep_for(150ms);
    auto s1 = chrono::steady_clock::now();
    cout << "Slept for: " << chrono::duration_cast<chrono::milliseconds>(s1 - s0).count() << " ms\n";

    // system_clock represents wall-clock time (calendar time). It can be adjusted by the OS.
    // Use system_clock when converting to human-readable calendar times.
    // system_clock 表示日历时间（墙钟时间），可能被操作系统调整。
    // 当需要转换为可读的日历时间时使用 system_clock。
    auto now_sys = chrono::system_clock::now();
    time_t now_time_t = chrono::system_clock::to_time_t(now_sys);
    cout << "Current system time: " << std::ctime(&now_time_t);

    // Duration arithmetic: chrono provides type-safe duration units and literals (C++14+).
    // You can add durations of different units; convert to desired unit with duration_cast.
    // 持续时间算术：chrono 提供了类型安全的时间单位与字面量（C++14 起）。
    // 可以对不同单位的持续时间做加法；使用 duration_cast 转换为目标单位。
    auto duration = 1h + 30min + 15s + 250ms;
    cout << "Duration example: " << timeutil::format_ms(chrono::duration_cast<chrono::milliseconds>(duration).count()) << "\n";

    cout << "now_ms(): " << timeutil::now_ms() << " ms since epoch (approx)\n";
}

// C time.h examples (ctime)
// C 风格的 time.h 示例
static void demo_ctime()
{
    cout << "\n=== C <time.h> (ctime) Demo ===\n";

    // time_t typically counts seconds since the epoch (1970-01-01 UTC on many platforms).
    // Be aware that the epoch and range of time_t can vary between platforms.
    // time_t 通常表示自 epoch（多数平台为 1970-01-01 UTC）以来的秒数；不同平台的 epoch/范围可能不同。
    time_t now = time(nullptr);
    cout << "time(nullptr) -> epoch seconds: " << now << "\n";

    // ctime returns a statically-allocated string (not thread-safe). It appends a newline.
    // Prefer ctime_r or localtime_r on POSIX, or ctime_s/localtime_s on MSVC for thread-safety.
    // ctime 返回静态字符串（非线程安全），并带有换行符。POSIX 下可使用 ctime_r/localtime_r，
    // MSVC 下可使用 ctime_s/localtime_s 来获得线程安全的替代。
    cout << "ctime(&now): " << ctime(&now);

    tm local_tm;
#if defined(_MSC_VER)
    localtime_s(&local_tm, &now);
#else
    local_tm = *localtime(&now);
#endif

    // put_time is a convenient C++ way to format tm structures (requires <iomanip>).
    // put_time 是格式化 tm 的方便方式（需要包含 <iomanip>）。
    cout << "Local time: " << put_time(&local_tm, "%Y-%m-%d %H:%M:%S") << "\n";

    // strftime formats broken-down time into a custom string buffer (C API).
    // strftime 将分解时间格式化到自定义缓冲区（C 接口）。
    char buf[128];
    strftime(buf, sizeof(buf), "%A, %d %B %Y %H:%M:%S", &local_tm);
    cout << "strftime: " << buf << "\n";

    // difftime measures wall-clock difference in seconds between two time_t values.
    // difftime 用于计算两个 time_t 值之间的墙钟时间差（以秒为单位）。
    time_t t1 = time(nullptr);
    this_thread::sleep_for(chrono::seconds(1));
    time_t t2 = time(nullptr);
    double elapsed_seconds = difftime(t2, t1);
    cout << "difftime after 1s sleep: " << elapsed_seconds << " seconds\n";

    // clock measures CPU time consumed by the program (not wall-clock). Result is in CLOCKS_PER_SEC units.
    // clock 用于测量程序占用的 CPU 时间（非墙钟时间），结果单位为 CLOCKS_PER_SEC。
    clock_t c0 = clock();
    volatile long long s = 0;
    for (long long i = 0; i < 20000000LL; ++i)
        s += i;
    clock_t c1 = clock();
    double cpu_time = double(c1 - c0) / CLOCKS_PER_SEC;
    cout << "CPU time for busy loop: " << cpu_time << " seconds\n";

    // mktime converts local broken-down time back to time_t (calendar time).
    // Note: mktime assumes the tm is local time and may modify tm's members (e.g., tm_wday, tm_yday).
    // mktime 将本地分解时间转换回 time_t（日历时间）。注意：mktime 假设 tm 为本地时间，
    // 并可能修改 tm 的某些成员（如 tm_wday、tm_yday）。
    tm copy_tm = local_tm;
    time_t reconstructed = mktime(&copy_tm);
    cout << "mktime(reconstructed) -> " << reconstructed << "\n";

    cout << "Note: <time.h> provides C-style APIs; in C++ prefer <chrono>.\n";
}

int main()
{
    demo_chrono();
    demo_ctime();
    cout << "Demo complete." << endl;
    return 0;
}