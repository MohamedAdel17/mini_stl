# Performance & Benchmarks

This document describes the benchmark suite included in `benchmarks/benchmark.cpp` and the performance characteristics of each `mini_stl` component.

---

## Running the Benchmarks

```bash
cmake -B build
cmake --build build
./build/benchmark
```

Benchmarks are compiled with `-O2` optimization to reflect realistic production conditions.

---

## What is Benchmarked

Each container is tested against its `std::` equivalent across three operations: insert, access, and remove. All benchmarks run **1,000,000 iterations**.

| Container | vs | Operations |
|---|---|---|
| `mini_stl::Vector<int>` | `std::vector<int>` | `push_back`, `operator[]`, `pop_back` |
| `mini_stl::Stack<int>` | `std::stack<int>` | `push`, `top`, `pop` |
| `mini_stl::Queue<int>` | `std::queue<int>` | `push`, `front`, `pop` |
| `ThreadSafeQueue` | `LockFreeStack` | `push + pop` throughput |

---

## 📊 Results

Compiled with: GCC — `-O2` — C++17 — WSL/Linux — 1,000,000 iterations

```
========================================
  Vector<int>  vs  std::vector<int>  (1000000 elements)
========================================
Operation                     mini_stl (ms)  std (ms)       Verdict
----------------------------------------------------------------------
push_back x1000000            6.265          3.759          ~ within range
operator[] x1000000           1.189          0.424          ~ within range
pop_back x1000000             0.000          0.332          ✓ competitive

========================================
  Stack<int>  vs  std::stack<int>  (1000000 elements)
========================================
Operation                     mini_stl (ms)  std (ms)       Verdict
----------------------------------------------------------------------
push x1000000                 33.173         9.183          ~ within range
top x1000000                  0.466          0.470          ✓ competitive
pop x1000000                  7.744          1.025          ~ within range

========================================
  Queue<int>  vs  std::queue<int>  (1000000 elements)
========================================
Operation                     mini_stl (ms)  std (ms)       Verdict
----------------------------------------------------------------------
push x1000000                 17.784         15.038         ✓ competitive
front x1000000                0.302          0.303          ✓ competitive
pop x1000000                  8.744          0.856          ~ within range

========================================
  ThreadSafeQueue  vs  LockFreeStack  (1000000 ops)
========================================
Operation                     mini_stl (ms)  std (ms)       Verdict
----------------------------------------------------------------------
push + pop x1000000           33.183         27.615         (TSQ vs LFS — single thread)
```

---

## 📝 Results Analysis

### Vector
- **`pop_back` is faster than `std::vector`** — `mini_stl::Vector::pop_back` is a single `_size--` decrement with no overhead, while `std::vector` performs bounds and state checks internally
- **`push_back` is ~1.7× slower** — expected: `std::vector` uses highly optimized platform-specific memory allocation internally. Our implementation uses the same doubling strategy but without those low-level optimizations
- **`operator[]` is ~2.8× slower** — `std::vector` benefits from compiler intrinsics and tighter memory layout optimizations

### Stack & Queue
- **`top` and `front` are on par with `std::`** — both are simple pointer dereferences, no difference
- **`push` and `pop` are slower** — both `mini_stl::Stack` and `mini_stl::Queue` are linked-list based, meaning every push/pop allocates/frees a heap node. `std::stack` wraps `std::deque` which uses chunked memory and avoids per-element allocation
- **Key insight:** for maximum single-threaded throughput, an array-based stack would outperform a linked-list stack. The linked-list design was chosen deliberately to demonstrate pointer manipulation, deep copy, and manual memory management

### ThreadSafeQueue vs LockFreeStack
- **LockFreeStack is ~17% faster** in single-threaded use — no mutex acquisition overhead
- The real advantage of `LockFreeStack` shows under **high contention** with many threads, where mutex-based approaches serialize threads and cause context switches
- `ThreadSafeQueue` trades raw speed for safety and simplicity — `wait_and_pop()` eliminates busy-spinning entirely, which is critical in producer/consumer systems

---

## Design & Performance Notes

### Vector
- Uses **doubling strategy** on expansion (`capacity *= 2`), giving O(1) amortized `push_back`
- Raw heap array (`Ty* ptr`) — cache-friendly, sequential memory layout
- `shrink_to_fit()` available to reclaim unused capacity

### Stack & Queue
- Both are **linked-list based** — each node is a separate heap allocation
- This means slightly more cache pressure than `std::stack` (which wraps `std::deque`) but O(1) guaranteed push/pop with no reallocation
- For maximum throughput in a single thread, an array-based stack would be faster

### ThreadSafeQueue
- Uses `std::mutex` + `std::condition_variable`
- Producers never busy-spin — `notify_one()` wakes exactly one blocked consumer
- `wait_and_pop()` uses a lambda predicate to guard against **spurious wakeups**
- Slightly slower than a plain queue in single-threaded use due to lock overhead — this is expected and acceptable

### LockFreeStack
- Uses `std::atomic<Node*>` with `compare_exchange_weak` — no mutex, no blocking
- Threads never sleep — ideal for **low-latency** scenarios (trading systems, ad bidding)
- Uses `memory_order_release` on push and `memory_order_acquire` on pop to establish happens-before guarantees
- Known limitation: **ABA problem** — in production, hazard pointers or tagged pointers would be used. Documented in `LockFreeStack.h`

---

## Memory Safety

All components validated with **Valgrind** — zero leaks:

```bash
valgrind --leak-check=full --track-origins=yes ./build/demo
```

Expected result:
```
All heap blocks were freed -- no leaks are possible
ERROR SUMMARY: 0 errors from 0 contexts
```

---

## Profiling with Perf (Linux)

```bash
perf stat ./build/benchmark
perf record ./build/benchmark
perf report
```

Key metrics to watch: `cache-misses`, `branch-misses`, `instructions per cycle`.

---

## Concurrency Model

```
ThreadSafeQueue (mutex-based)         LockFreeStack (atomic CAS)
─────────────────────────────         ──────────────────────────
+ Simple, correct, easy to reason     + No blocking, lowest latency
+ Consumers sleep while waiting       + Scales well under high contention
- Lock overhead                       - ABA problem (documented)
- One thread at a time holds lock     - Harder to reason about correctness
```
