# Contributing to mini_stl

Thank you for your interest in contributing! This document covers code style, how to add a new container, and the code review process.

---

## 📋 Table of Contents

- [Code Style](#code-style)
- [Adding a New Container](#adding-a-new-container)
- [Code Review Guidelines](#code-review-guidelines)
- [Commit Message Format](#commit-message-format)
- [Pull Request Checklist](#pull-request-checklist)

---

## 🎨 Code Style

### General

- Use **C++17** features where appropriate
- All code lives inside `namespace mini_stl { }`
- Every file starts with `#pragma once`
- Never use `using namespace std;` inside a header file — it pollutes the includer's namespace
- Prefer `const` correctness — mark every method that doesn't modify state as `const`

### Naming Conventions

| Thing | Convention | Example |
|---|---|---|
| Classes | PascalCase | `ThreadSafeQueue` |
| Methods | snake_case | `push_back`, `try_pop` |
| Private members | snake_case with `_` prefix | `_size`, `_mutex` |
| Template parameters | PascalCase starting with `Ty` | `Ty`, `Ty1`, `Ty2` |
| Node structs | PascalCase | `Node`, `NodeAVL` |
| Local variables | snake_case | `old_head`, `new_node` |

### Memory Management

- Every `new` must have a corresponding `delete` — always implement a destructor
- Always implement the **Rule of Three**: destructor, copy constructor, copy assignment operator
- For concurrent containers, delete copy constructor and assignment operator — copying a live mutex is undefined behavior
- Use RAII — acquire resources in constructors, release in destructors

### Comments

- Add a block comment at the top of each class explaining what it does and what concurrency primitives it uses (if any)
- Comment non-obvious algorithmic decisions inline
- For lock-free code, comment every `memory_order_*` choice explaining why that ordering was chosen

### Example of a well-formatted header

```cpp
#pragma once

#include <stdexcept>

namespace mini_stl
{
    // -------------------------------------------------------
    // MyContainer<Ty>
    //
    // Brief description of what this does.
    // Any concurrency notes go here.
    // -------------------------------------------------------

    template <typename Ty>
    class MyContainer
    {
        struct Node
        {
            Ty data;
            Node *next;
            Node(const Ty &val) : data(val), next(nullptr) {}
        };

        Node *head;
        unsigned long long _size;

    public:
        MyContainer() : head(nullptr), _size(0) {}
        ~MyContainer() { clear(); }

        // Non-copyable example (for concurrent containers)
        // MyContainer(const MyContainer &) = delete;
        // MyContainer &operator=(const MyContainer &) = delete;

        void push(const Ty &val);
        bool try_pop(Ty &out);

        bool empty() const { return head == nullptr; }
        unsigned long long size() const { return _size; }
        void clear();
    };

} // namespace mini_stl
```

---

## ➕ Adding a New Container

Follow these steps when adding a new data structure:

**1. Create the header** in `include/mini_stl/MyContainer.h`
- Follow the code style above
- Implement at minimum: default constructor, destructor, copy constructor, copy assignment, `empty()`, `size()`, `clear()`
- Add iterators (`begin()` / `end()`) if the container is traversable

**2. Add it to the demo** in `examples/demo.cpp`
- Add a `section("MyContainer")` block
- Show the most important operations with `std::cout` output

**3. Add it to the benchmark** in `benchmarks/benchmark.cpp` (if performance-relevant)
- Compare against the closest `std::` equivalent
- Test at least: insert, access, remove

**4. Update the README**
- Add a row to the Components table
- Update the Project Structure tree

**5. Update PERFORMANCE.md** if you added benchmarks

**6. Run Valgrind** before submitting:
```bash
valgrind --leak-check=full --track-origins=yes ./build/demo
```
Zero leaks expected.

---

## 🔍 Code Review Guidelines

When reviewing a pull request, check for:

### Correctness
- [ ] Does it handle empty container edge cases?
- [ ] Are there any memory leaks? (check destructor and all early returns)
- [ ] Does the copy constructor produce a deep copy, not a shallow one?
- [ ] Are exceptions thrown for invalid operations (e.g. `top()` on empty stack)?

### Concurrency (for concurrent containers)
- [ ] Is every shared variable protected by a mutex or atomic?
- [ ] Are condition variable predicates using lambda guards against spurious wakeups?
- [ ] Is the memory ordering correct and documented?
- [ ] Is the container correctly marked non-copyable?

### Performance
- [ ] Are there any unnecessary copies? Prefer `const Ty &` parameters
- [ ] Does the algorithm match the expected time complexity?
- [ ] Is heap allocation minimized in hot paths?

### Style
- [ ] Does it follow naming conventions?
- [ ] Is `#pragma once` at the top?
- [ ] Is everything inside `namespace mini_stl`?
- [ ] No `using namespace std` in headers?

---

## 📝 Commit Message Format

Use clear, descriptive commit messages:

```
<type>: <short description>

<optional longer explanation>
```

Types:
- `feat` — new container or feature
- `fix` — bug fix
- `perf` — performance improvement
- `docs` — documentation update
- `refactor` — code restructure without behavior change
- `test` — adding benchmarks or tests

Examples:
```
feat: add ThreadSafeQueue with wait_and_pop
fix: fix memory leak in AVL erase when node has two children
perf: optimize Vector expand to use memcpy for trivial types
docs: add Valgrind results to PERFORMANCE.md
```

---

## ✅ Pull Request Checklist

Before opening a PR, confirm:

- [ ] Code compiles with no warnings (`-Wall -Wextra -Wpedantic`)
- [ ] Valgrind reports zero leaks on `./build/demo`
- [ ] New container is added to `demo.cpp` with visible output
- [ ] README components table is updated
- [ ] Commit messages follow the format above

---

## 👤 Maintainer

**Mohamed Adel** — [github.com/MohamedAdel17](https://github.com/MohamedAdel17)
