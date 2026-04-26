# 🧰 mini_stl

A lightweight, **header-only C++ library** that re-implements core data structures and algorithms from scratch — inspired by the C++ Standard Template Library (STL).

Built as a learning and portfolio project to demonstrate a deep understanding of data structures, memory management, and generic programming in C++.

---

## 📦 Components

| Component | Header | Description |
|-----------|--------|-------------|
| Vector | `mini_stl/Vector.h` | Dynamic array with automatic resizing |
| Singly Linked List | `mini_stl/SLL.h` | Linear list with forward traversal |
| Doubly Linked List | `mini_stl/DLL.h` | Linear list with forward & backward traversal |
| Stack | `mini_stl/Stack.h` | LIFO data structure |
| Queue | `mini_stl/Queue.h` | FIFO data structure |
| Binary Search Tree | `mini_stl/BST.h` | Ordered binary tree with search & traversal |
| AVL Tree | `mini_stl/AVL.h` | Self-balancing BST with automatic rotations |
| Map | `mini_stl/Map.h` | Key-value store built on a BST |

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

### Build & run the demo

```bash
cmake -B build
cmake --build build
./build/demo
```

---

## 🧪 Usage Example

Since this is a header-only library, just include the header you need — no linking required.

```cpp
#include "mini_stl/Vector.h"
#include "mini_stl/Stack.h"
#include "mini_stl/BST.h"

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

    // BST
    mini_stl::BST<int> bst;
    bst.insert(50);
    bst.insert(30);
    bst.insert(70);
    bst.inorder();

    return 0;
}
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
│       └── Map.h
├── examples/
│   └── demo.cpp
├── CMakeLists.txt
└── README.md
```

---

## 💡 Design Decisions

- **Header-only** — every class lives entirely in its `.h` file, so there's nothing to compile or link. Just `#include` and go.
- **Namespace** — all classes are wrapped in `namespace mini_stl` to avoid name collisions with the standard library.
- **Templates** — all containers are generic and work with any data type.
- **No external dependencies** — pure C++17, nothing else needed.

---

## 👤 Author

**Mohamed Adel**
[github.com/MohamedAdel17](https://github.com/MohamedAdel17)

---

## 📄 License

This project is open source and available under the [MIT License](LICENSE).
