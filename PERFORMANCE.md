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

All components have been validated with **Valgrind**:

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
