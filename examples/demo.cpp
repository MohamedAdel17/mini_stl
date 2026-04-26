#include <iostream>
#include "mini_stl/Vector.h"
#include "mini_stl/SLL.h"
#include "mini_stl/DLL.h"
#include "mini_stl/Stack.h"
#include "mini_stl/Queue.h"
#include "mini_stl/BST.h"
#include "mini_stl/AVL.h"
#include "mini_stl/Map.h"

// -------------------------------------------------------
// Helper: print a section header
// -------------------------------------------------------
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
    std::cout << "Size: " << vec.size() << "\n";
    std::cout << "Element at index 2: " << vec[2] << "\n";
    vec.pop_back();
    std::cout << "After pop_back, size: " << vec.size() << "\n";

    // -------------------------------------------------------
    // Singly Linked List
    // -------------------------------------------------------
    section("Singly Linked List (SLL)");
    mini_stl::SLL<int> sll;
    sll.push_front(3);
    sll.push_front(2);
    sll.push_front(1);
    std::cout << "Pushed front: 1, 2, 3\n";
    std::cout << "List: ";
    for (auto it = sll.begin(); it != sll.end(); ++it)
    {
        std::cout << *it << " ";
    }
    sll.pop_front();
    std::cout << "After pop_front: ";
    for (const auto &val : sll)
    {
        std::cout << val << " ";
    }
    std::cout << "\n";

    // -------------------------------------------------------
    // Doubly Linked List
    // -------------------------------------------------------
    section("Doubly Linked List (DLL)");
    mini_stl::DLL<int> dll;
    dll.push_back(100);
    dll.push_back(200);
    dll.push_back(300);
    std::cout << "Pushed back: 100, 200, 300\n";
    std::cout << "Forward:  ";
    for (auto it = dll.begin(); it != dll.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << "\n";
    std::cout << "Backward: ";
    for (auto it = dll.rbegin(); it != dll.rend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // -------------------------------------------------------
    // Stack
    // -------------------------------------------------------
    section("Stack");
    mini_stl::Stack<int> stk;
    stk.push(5);
    stk.push(10);
    stk.push(15);
    std::cout << "Pushed: 5, 10, 15\n";
    std::cout << "Top: " << stk.top() << "\n";
    stk.pop();
    std::cout << "After pop, top: " << stk.top() << "\n";
    std::cout << "Is empty: " << (stk.empty() ? "yes" : "no") << "\n";

    // -------------------------------------------------------
    // Queue
    // -------------------------------------------------------
    section("Queue");
    mini_stl::Queue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);
    std::cout << "Enqueued: 1, 2, 3\n";
    std::cout << "Front: " << q.front() << "\n";
    q.pop();
    std::cout << "After dequeue, front: " << q.front() << "\n";

    // -------------------------------------------------------
    // Binary Search Tree
    // -------------------------------------------------------
    section("Binary Search Tree (BST)");
    mini_stl::BST<int> bst;
    bst.insert(50);
    bst.insert(30);
    bst.insert(70);
    bst.insert(20);
    bst.insert(40);
    std::cout << "Inserted: 50, 30, 70, 20, 40\n";
    std::cout << "In-order traversal: ";
    for (auto it = bst.begin(); it != bst.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << "\n";
    std::cout << "Search 30: " << (bst.find(30) != bst.end() ? "found" : "not found") << "\n";
    std::cout << "Search 99: " << (bst.find(99) != bst.end() ? "found" : "not found") << "\n";

    // -------------------------------------------------------
    // AVL Tree
    // -------------------------------------------------------
    section("AVL Tree");
    mini_stl::AVL<int> avl;
    avl.insert(10);
    avl.insert(20);
    avl.insert(30); // triggers rotation
    avl.insert(40);
    avl.insert(50); // triggers rotation
    std::cout << "Inserted: 10, 20, 30, 40, 50 (with auto-balancing)\n";
    std::cout << "In-order traversal: ";
    for (auto it = avl.begin(); it != avl.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // -------------------------------------------------------
    // Map
    // -------------------------------------------------------
    section("Map");
    mini_stl::Map<std::string, int> map;
    map.insert({"Alice", 90});
    map.insert({"Bob", 75});
    map.insert({"Carol", 88});
    std::cout << "Inserted: Alice=90, Bob=75, Carol=88\n";
    std::cout << "Alice's score: " << map["Alice"] << "\n";
    std::cout << "Bob's score:   " << map["Bob"] << "\n";
    std::cout << "Contains 'Carol': " << (map.find("Carol") != map.end() ? "yes" : "no") << "\n";
    std::cout << "Contains 'Dave':  " << (map.find("Dave") != map.end() ? "yes" : "no") << "\n";

    // -------------------------------------------------------
    std::cout << "\n========================================\n";
    std::cout << "  All demos completed successfully!\n";
    std::cout << "========================================\n\n";

    return 0;
}
