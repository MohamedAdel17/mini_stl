# 🧰 mini_stl

A lightweight, **header-only C++ library** that re-implements core data structures and algorithms from scratch — inspired by the C++ Standard Template Library (STL).

Built as a learning and portfolio project to demonstrate a deep understanding of data structures, memory management, generic programming, and concurrent systems in C++.

---

## 📦 Components

### Core Containers

| Component | Header | Description |
|-----------|--------|-------------|
| Vector | `mini_stl/Vector.h` | Dynamic array with automatic resizing |
| Singly Linked List | `mini_stl/SLL.h` | Linear list with forward traversal |
| Doubly Linked List | `mini_stl/DLL.h` | Linear list with forward & backward traversal |
| Stack | `mini_stl/Stack.h` | LIFO data structure |
| Queue | `mini_stl/Queue.h` | FIFO data structure |
| Binary Search Tree | `mini_stl/BST.h` | Ordered binary tree with search, traversal & balancing |
| AVL Tree | `mini_stl/AVL.h` | Self-balancing BST with automatic rotations |
| Map | `mini_stl/Map.h` | Key-value store built on a BST |

### Concurrent Containers

| Component | Header | Description |
|-----------|--------|-------------|
| ThreadSafeQueue | `mini_stl/ThreadSafeQueue.h` | Mutex-based queue with blocking `wait_and_pop` |
| LockFreeStack | `mini_stl/LockFreeStack.h` | Lock-free stack using `std::atomic` and CAS |

---

## 🚀 Getting Started

### Requirements

- C++17 or later
- CMake 3.15+
- A C++ compiler (GCC, Clang, or MSVC)

### Clone the repo

```bash
git clone https://github.com/MohamedAdel17/mini_stl.git
cd mini_stl
```

### Build

```bash
cmake -B build
cmake --build build
```

### Run the demo

```bash
./build/demo
```

### Run the benchmarks

```bash
./build/benchmark
```

---

## 🧪 Usage Example

Since this is a header-only library, just include the header you need — no linking required.

```cpp
#include "mini_stl/Vector.h"
#include "mini_stl/Stack.h"
#include "mini_stl/ThreadSafeQueue.h"
#include "mini_stl/LockFreeStack.h"

int main() {
    // Vector
    mini_stl::Vector<int> vec;
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);

    // Stack
    mini_stl::Stack<int> stk;
    stk.push(5);
    stk.push(10);
    stk.pop();

    // Thread-safe queue — producer/consumer safe
    mini_stl::ThreadSafeQueue<int> tsq;
    tsq.push(42);
    int val;
    tsq.wait_and_pop(val);  // blocks until item is available

    // Lock-free stack — no mutex, lowest latency
    mini_stl::LockFreeStack<int> lfs;
    lfs.push(99);
    lfs.try_pop(val);

    return 0;
}
```

---

## ⚡ Concurrency

### ThreadSafeQueue
- Multiple producer / multiple consumer safe
- Uses `std::mutex` + `std::condition_variable`
- `wait_and_pop()` blocks the calling thread until data is available — no busy-spinning
- `try_pop()` returns immediately if the queue is empty

### LockFreeStack
- Uses `std::atomic<Node*>` with `compare_exchange_weak`
- No mutex — threads never block, ideal for low-latency systems
- Documented ABA limitation in header (production fix: hazard pointers)
- Uses `memory_order_release` / `memory_order_acquire` for correct happens-before guarantees

---

## 📊 Benchmarks

The benchmark suite (`benchmarks/benchmark.cpp`) measures each container against its `std::` equivalent across 1,000,000 operations.

```bash
./build/benchmark
```

See [PERFORMANCE.md](PERFORMANCE.md) for full details on methodology, results, and design tradeoffs.

---

## ✅ Memory Safety

All components validated with Valgrind — zero leaks:

```bash
valgrind --leak-check=full --track-origins=yes ./build/demo
```

```
All heap blocks were freed -- no leaks are possible
ERROR SUMMARY: 0 errors from 0 contexts
```

---

## 📁 Project Structure

```
mini_stl/
├── include/
│   └── mini_stl/
│       ├── Vector.h
│       ├── SLL.h
│       ├── DLL.h
│       ├── Stack.h
│       ├── Queue.h
│       ├── BST.h
│       ├── AVL.h
│       ├── Map.h
│       ├── ThreadSafeQueue.h
│       └── LockFreeStack.h
├── examples/
│   └── demo.cpp
├── benchmarks/
│   └── benchmark.cpp
├── CMakeLists.txt
├── PERFORMANCE.md
└── README.md
```

---

## 💡 Design Decisions

- **Header-only** — every class lives entirely in its `.h` file, so there's nothing to compile or link. Just `#include` and go.
- **Namespace** — all classes are wrapped in `namespace mini_stl` to avoid name collisions with the standard library.
- **Templates** — all containers are generic and work with any data type.
- **Iterators** — core containers implement `begin()`/`end()` for range-based for loop support.
- **Concurrency** — concurrent containers are non-copyable by design (copying a live mutex or atomic is undefined behavior).
- **No external dependencies** — pure C++17, nothing else needed.

---

## 👤 Author

**Mohamed Adel**
[github.com/MohamedAdel17](https://github.com/MohamedAdel17)

---

## 📄 License

This project is open source and available under the [MIT License](LICENSE).
