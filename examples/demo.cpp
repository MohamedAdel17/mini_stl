#include <iostream>
#include <thread>
#include <vector>

#include "mini_stl/Vector.h"
#include "mini_stl/SLL.h"
#include "mini_stl/DLL.h"
#include "mini_stl/Stack.h"
#include "mini_stl/Queue.h"
#include "mini_stl/BST.h"
#include "mini_stl/AVL.h"
#include "mini_stl/Map.h"
#include "mini_stl/ThreadSafeQueue.h"
#include "mini_stl/LockFreeStack.h"

void section(const std::string &title)
{
    std::cout << "\n========================================\n";
    std::cout << "  " << title << "\n";
    std::cout << "========================================\n";
}

int main()
{
    // -------------------------------------------------------
    // Vector
    // -------------------------------------------------------
    section("Vector");
    mini_stl::Vector<int> vec;
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);
    vec.push_back(40);
    std::cout << "Pushed: 10, 20, 30, 40\n";
    std::cout << "Size: "            << vec.size()     << "\n";
    std::cout << "Capacity: "        << vec.capacity() << "\n";
    std::cout << "Element at [2]: "  << vec[2]         << "\n";
    vec.pop_back();
    std::cout << "After pop_back, size: " << vec.size() << "\n";
    vec.shrink_to_fit();
    std::cout << "After shrink_to_fit, capacity: " << vec.capacity() << "\n";

    // -------------------------------------------------------
    // Singly Linked List
    // -------------------------------------------------------
    section("Singly Linked List (SLL)");
    mini_stl::SLL<int> sll;
    sll.push_back(1);
    sll.push_back(2);
    sll.push_back(3);
    std::cout << "Pushed back: 1, 2, 3\n";
    std::cout << "Front: " << sll.front() << "\n";
    std::cout << "Size:  " << sll.size()  << "\n";
    sll.reverse();
    std::cout << "After reverse, front: " << sll.front() << "\n";
    sll.pop_front();
    std::cout << "After pop_front, size: " << sll.size() << "\n";

    // -------------------------------------------------------
    // Doubly Linked List
    // -------------------------------------------------------
    section("Doubly Linked List (DLL)");
    mini_stl::DLL<int> dll;
    dll.push_back(100);
    dll.push_back(200);
    dll.push_back(300);
    std::cout << "Pushed back: 100, 200, 300\n";
    std::cout << "Front: " << dll.front() << "\n";
    std::cout << "Back:  " << dll.back()  << "\n";
    std::cout << "Size:  " << dll.size()  << "\n";
    dll.push_front(0);
    std::cout << "After push_front(0), front: " << dll.front() << "\n";
    dll.reverse();
    std::cout << "After reverse, front: " << dll.front() << "\n";

    // -------------------------------------------------------
    // Stack
    // -------------------------------------------------------
    section("Stack");
    mini_stl::Stack<int> stk;
    stk.push(5);
    stk.push(10);
    stk.push(15);
    std::cout << "Pushed: 5, 10, 15\n";
    std::cout << "Top:  " << stk.top()  << "\n";
    std::cout << "Size: " << stk.size() << "\n";
    stk.pop();
    std::cout << "After pop, top: "  << stk.top()  << "\n";
    std::cout << "Is empty: " << (stk.empty() ? "yes" : "no") << "\n";

    // -------------------------------------------------------
    // Queue
    // -------------------------------------------------------
    section("Queue");
    mini_stl::Queue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);
    std::cout << "Pushed: 1, 2, 3\n";
    std::cout << "Front: " << q.front() << "\n";
    std::cout << "Back:  " << q.back()  << "\n";
    std::cout << "Size:  " << q.size()  << "\n";
    q.pop();
    std::cout << "After pop, front: " << q.front() << "\n";

    // -------------------------------------------------------
    // BST
    // -------------------------------------------------------
    section("Binary Search Tree (BST)");
    mini_stl::BST<int> bst;
    bst.insert(50);
    bst.insert(30);
    bst.insert(70);
    bst.insert(20);
    bst.insert(40);
    std::cout << "Inserted: 50, 30, 70, 20, 40\n";
    std::cout << "In-order:  ";
    for (auto it = bst.begin(); it != bst.end(); ++it)
        std::cout << *it << " ";
    std::cout << "\n";
    auto found = bst.find(30);
    std::cout << "find(30): " << (*found) << "\n";
    bst.erase(30);
    std::cout << "After erase(30), in-order: ";
    for (auto it = bst.begin(); it != bst.end(); ++it)
        std::cout << *it << " ";
    std::cout << "\n";

    // -------------------------------------------------------
    // AVL
    // -------------------------------------------------------
    section("AVL Tree");
    mini_stl::AVL<int> avl;
    avl.insert(10);
    avl.insert(20);
    avl.insert(30); // triggers right rotation
    avl.insert(40);
    avl.insert(50); // triggers right rotation
    std::cout << "Inserted: 10, 20, 30, 40, 50 (auto-balanced)\n";
    std::cout << "In-order: ";
    for (auto it = avl.begin(); it != avl.end(); ++it)
        std::cout << *it << " ";
    std::cout << "\n";
    auto avl_found = avl.find(30);
    std::cout << "find(30): " << (*avl_found) << "\n";
    avl.erase(30);
    std::cout << "After erase(30), in-order: ";
    for (auto it = avl.begin(); it != avl.end(); ++it)
        std::cout << *it << " ";
    std::cout << "\n";

    // -------------------------------------------------------
    // Map
    // -------------------------------------------------------
    section("Map");
    mini_stl::Map<std::string, int> map;
    map.insert({"Alice", 90});
    map.insert({"Bob",   75});
    map.insert({"Carol", 88});
    std::cout << "Inserted: Alice=90, Bob=75, Carol=88\n";
    std::cout << "Alice: " << map["Alice"] << "\n";
    std::cout << "Bob:   " << map["Bob"]   << "\n";
    map["Dave"] = 95; // insert via operator[]
    std::cout << "Dave (via operator[]): " << map["Dave"] << "\n";
    auto mit = map.find("Carol");
    std::cout << "find(Carol): " << mit->second << "\n";
    map.erase("Bob");
    std::cout << "After erase(Bob), find(Bob): "
              << (map.find("Bob") == map.end() ? "not found" : "found") << "\n";

    // -------------------------------------------------------
    // ThreadSafeQueue — producer/consumer with std::thread
    // -------------------------------------------------------
    section("ThreadSafeQueue (multi-threaded)");
    mini_stl::ThreadSafeQueue<int> tsq;

    // Producer thread: pushes 1..5
    std::thread producer([&tsq] {
        for (int i = 1; i <= 5; i++)
        {
            tsq.push(i);
            std::cout << "  [producer] pushed " << i << "\n";
        }
    });

    // Consumer thread: pops 5 items using blocking wait
    std::thread consumer([&tsq] {
        for (int i = 0; i < 5; i++)
        {
            int val;
            tsq.wait_and_pop(val);
            std::cout << "  [consumer] popped " << val << "\n";
        }
    });

    producer.join();
    consumer.join();
    std::cout << "ThreadSafeQueue size after demo: " << tsq.size() << "\n";

    // -------------------------------------------------------
    // LockFreeStack — concurrent pushes then pops
    // -------------------------------------------------------
    section("LockFreeStack (multi-threaded)");
    mini_stl::LockFreeStack<int> lfs;

    // Two threads pushing concurrently
    std::thread t1([&lfs] {
        for (int i = 0; i < 3; i++) lfs.push(i * 10);
    });
    std::thread t2([&lfs] {
        for (int i = 0; i < 3; i++) lfs.push(i * 100);
    });
    t1.join();
    t2.join();

    std::cout << "After 2 threads pushing 3 items each, size: " << lfs.size() << "\n";
    int out;
    std::cout << "Popping all: ";
    while (lfs.try_pop(out))
        std::cout << out << " ";
    std::cout << "\n";

    // -------------------------------------------------------
    std::cout << "\n========================================\n";
    std::cout << "  All demos completed successfully!\n";
    std::cout << "========================================\n\n";

    return 0;
}
