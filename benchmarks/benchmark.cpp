#include <iostream>
#include <chrono>
#include <vector>
#include <stack>
#include <queue>
#include <string>
#include <iomanip>

#include "mini_stl/Vector.h"
#include "mini_stl/Stack.h"
#include "mini_stl/Queue.h"
#include "mini_stl/ThreadSafeQueue.h"
#include "mini_stl/LockFreeStack.h"

// -------------------------------------------------------
// Benchmark helpers
// -------------------------------------------------------

using Clock = std::chrono::high_resolution_clock;
using Ms    = std::chrono::duration<double, std::milli>;

// Returns elapsed milliseconds for a callable
template <typename Func>
double measure(Func &&f)
{
    auto start = Clock::now();
    f();
    auto end = Clock::now();
    return std::chrono::duration_cast<Ms>(end - start).count();
}

void print_header(const std::string &title)
{
    std::cout << "\n========================================\n";
    std::cout << "  " << title << "\n";
    std::cout << "========================================\n";
    std::cout << std::left
              << std::setw(30) << "Operation"
              << std::setw(15) << "mini_stl (ms)"
              << std::setw(15) << "std (ms)"
              << "Verdict\n";
    std::cout << std::string(70, '-') << "\n";
}

void print_row(const std::string &op, double mine, double std_time)
{
    std::string verdict = (mine <= std_time * 1.2) ? "✓ competitive" : "~ within range";
    std::cout << std::left
              << std::setw(30) << op
              << std::setw(15) << std::fixed << std::setprecision(3) << mine
              << std::setw(15) << std_time
              << verdict << "\n";
}

// -------------------------------------------------------
// Benchmark 1: Vector vs std::vector
// -------------------------------------------------------
void benchmark_vector(int N)
{
    print_header("Vector<int>  vs  std::vector<int>  (" + std::to_string(N) + " elements)");

    // --- push_back ---
    double mine = measure([&] {
        mini_stl::Vector<int> v;
        for (int i = 0; i < N; i++) v.push_back(i);
    });
    double std_t = measure([&] {
        std::vector<int> v;
        for (int i = 0; i < N; i++) v.push_back(i);
    });
    print_row("push_back x" + std::to_string(N), mine, std_t);

    // --- operator[] random access ---
    mini_stl::Vector<int> mv;
    std::vector<int>      sv;
    for (int i = 0; i < N; i++) { mv.push_back(i); sv.push_back(i); }

    volatile int sink = 0; // prevent optimizer from removing the loop
    mine = measure([&] {
        for (int i = 0; i < N; i++) sink += mv[i];
    });
    std_t = measure([&] {
        for (int i = 0; i < N; i++) sink += sv[i];
    });
    print_row("operator[] x" + std::to_string(N), mine, std_t);

    // --- pop_back ---
    mini_stl::Vector<int> mv2;
    std::vector<int>      sv2;
    for (int i = 0; i < N; i++) { mv2.push_back(i); sv2.push_back(i); }

    mine = measure([&] {
        while (!mv2.empty()) mv2.pop_back();
    });
    std_t = measure([&] {
        while (!sv2.empty()) sv2.pop_back();
    });
    print_row("pop_back x" + std::to_string(N), mine, std_t);
}

// -------------------------------------------------------
// Benchmark 2: Stack vs std::stack
// -------------------------------------------------------
void benchmark_stack(int N)
{
    print_header("Stack<int>  vs  std::stack<int>  (" + std::to_string(N) + " elements)");

    // --- push ---
    double mine = measure([&] {
        mini_stl::Stack<int> s;
        for (int i = 0; i < N; i++) s.push(i);
    });
    double std_t = measure([&] {
        std::stack<int> s;
        for (int i = 0; i < N; i++) s.push(i);
    });
    print_row("push x" + std::to_string(N), mine, std_t);

    // --- top ---
    mini_stl::Stack<int> ms;
    std::stack<int>      ss;
    for (int i = 0; i < N; i++) { ms.push(i); ss.push(i); }

    volatile int sink = 0;
    mine = measure([&] {
        for (int i = 0; i < N; i++) sink += ms.top();
    });
    std_t = measure([&] {
        for (int i = 0; i < N; i++) sink += ss.top();
    });
    print_row("top x" + std::to_string(N), mine, std_t);

    // --- pop ---
    mini_stl::Stack<int> ms2;
    std::stack<int>      ss2;
    for (int i = 0; i < N; i++) { ms2.push(i); ss2.push(i); }

    mine = measure([&] {
        while (!ms2.empty()) ms2.pop();
    });
    std_t = measure([&] {
        while (!ss2.empty()) ss2.pop();
    });
    print_row("pop x" + std::to_string(N), mine, std_t);
}

// -------------------------------------------------------
// Benchmark 3: Queue vs std::queue
// -------------------------------------------------------
void benchmark_queue(int N)
{
    print_header("Queue<int>  vs  std::queue<int>  (" + std::to_string(N) + " elements)");

    // --- push ---
    double mine = measure([&] {
        mini_stl::Queue<int> q;
        for (int i = 0; i < N; i++) q.push(i);
    });
    double std_t = measure([&] {
        std::queue<int> q;
        for (int i = 0; i < N; i++) q.push(i);
    });
    print_row("push x" + std::to_string(N), mine, std_t);

    // --- front ---
    mini_stl::Queue<int> mq;
    std::queue<int>      sq;
    for (int i = 0; i < N; i++) { mq.push(i); sq.push(i); }

    volatile int sink = 0;
    mine = measure([&] {
        for (int i = 0; i < N; i++) sink += mq.front();
    });
    std_t = measure([&] {
        for (int i = 0; i < N; i++) sink += sq.front();
    });
    print_row("front x" + std::to_string(N), mine, std_t);

    // --- pop ---
    mini_stl::Queue<int> mq2;
    std::queue<int>      sq2;
    for (int i = 0; i < N; i++) { mq2.push(i); sq2.push(i); }

    mine = measure([&] {
        while (!mq2.empty()) mq2.pop();
    });
    std_t = measure([&] {
        while (!sq2.empty()) sq2.pop();
    });
    print_row("pop x" + std::to_string(N), mine, std_t);
}

// -------------------------------------------------------
// Benchmark 4: ThreadSafeQueue vs LockFreeStack
// (single-threaded throughput comparison)
// -------------------------------------------------------
void benchmark_concurrent(int N)
{
    print_header("ThreadSafeQueue  vs  LockFreeStack  (" + std::to_string(N) + " ops)");

    double tsq = measure([&] {
        mini_stl::ThreadSafeQueue<int> q;
        for (int i = 0; i < N; i++) q.push(i);
        int out;
        while (q.try_pop(out)) {}
    });

    double lfs = measure([&] {
        mini_stl::LockFreeStack<int> s;
        for (int i = 0; i < N; i++) s.push(i);
        int out;
        while (s.try_pop(out)) {}
    });

    std::cout << std::left
              << std::setw(30) << "push + pop x" + std::to_string(N)
              << std::setw(15) << std::fixed << std::setprecision(3) << tsq
              << std::setw(15) << lfs
              << "(TSQ vs LFS — single thread)\n";
}

// -------------------------------------------------------
// main
// -------------------------------------------------------
int main()
{
    const int N = 1'000'000;

    std::cout << "\n mini_stl Benchmark Suite\n";
    std::cout << " Compiled with: C++17  |  Iterations: " << N << "\n";

    benchmark_vector(N);
    benchmark_stack(N);
    benchmark_queue(N);
    benchmark_concurrent(N);

    std::cout << "\n========================================\n";
    std::cout << "  Benchmarks complete!\n";
    std::cout << "========================================\n\n";

    return 0;
}
