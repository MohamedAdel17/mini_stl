#pragma once

#include <atomic>
#include <stdexcept>

namespace mini_stl
{

    // -------------------------------------------------------
    // LockFreeStack<Ty>
    //
    // A lock-free LIFO stack using std::atomic.
    // Multiple threads can push/pop concurrently without
    // any mutex — no thread ever blocks waiting for a lock.
    //
    // Key concurrency primitive used:
    //   - std::atomic<Node*>         : atomic pointer to head
    //   - compare_exchange_weak      : the heart of lock-free
    //                                  programming — atomically
    //                                  "check and swap" the head
    //                                  pointer only if it hasn't
    //                                  changed since we last read it
    //
    // How compare_exchange_weak works:
    //   expected = old_head;
    //   if (head == expected)        // no other thread changed it
    //       head = new_node;         // we win, do the swap
    //   else
    //       expected = head;         // someone else changed it,
    //                                // retry with the new value
    //
    // NOTE: This implementation has the classic ABA problem.
    //       For a production system you'd use hazard pointers
    //       or tagged pointers. This is intentionally kept
    //       clean for readability and portfolio demonstration.
    // -------------------------------------------------------

    template <typename Ty>
    class LockFreeStack
    {
        struct Node
        {
            Ty data;
            Node *next;
            Node(const Ty &val) : data(val), next(nullptr) {}
        };

        // The only shared state — an atomic pointer to the top node
        std::atomic<Node *> head;
        std::atomic<unsigned long long> _size;

    public:

        // Default constructor
        LockFreeStack() : head(nullptr), _size(0) {}

        // Destructor
        ~LockFreeStack() { clear(); }

        // Non-copyable — atomics cannot be copied
        LockFreeStack(const LockFreeStack &) = delete;
        LockFreeStack &operator=(const LockFreeStack &) = delete;

        // -------------------------------------------------------
        // push — lock-free push to top
        //
        // 1. Create new node pointing to current head
        // 2. Try to swing head to new node atomically
        // 3. If another thread changed head between steps 1 and 2,
        //    compare_exchange_weak updates old_head and we retry
        // -------------------------------------------------------
        void push(const Ty &val)
        {
            Node *new_node = new Node(val);

            // Load current head — relaxed is fine here since
            // compare_exchange provides the necessary ordering
            new_node->next = head.load(std::memory_order_relaxed);

            // Retry loop — exits as soon as our CAS succeeds
            while (!head.compare_exchange_weak(
                new_node->next,     // expected: if head == this, swap
                new_node,           // desired:  set head to new_node
                std::memory_order_release,  // success ordering
                std::memory_order_relaxed)) // failure ordering
            {
                // compare_exchange_weak already updated new_node->next
                // to the current head value on failure — just retry
            }

            _size.fetch_add(1, std::memory_order_relaxed);
        }

        // -------------------------------------------------------
        // try_pop — lock-free pop from top
        // Returns false if stack is empty
        //
        // 1. Read current head
        // 2. Try to swing head to head->next atomically
        // 3. If another thread popped between steps 1 and 2,
        //    old_head is updated and we retry
        // -------------------------------------------------------
        bool try_pop(Ty &out)
        {
            Node *old_head = head.load(std::memory_order_relaxed);

            while (old_head)
            {
                if (head.compare_exchange_weak(
                    old_head,               // expected
                    old_head->next,         // desired: skip past old head
                    std::memory_order_acquire,
                    std::memory_order_relaxed))
                {
                    // We successfully claimed old_head
                    out = old_head->data;
                    delete old_head;
                    _size.fetch_sub(1, std::memory_order_relaxed);
                    return true;
                }
                // old_head was updated to current head — retry
            }

            return false; // stack was empty
        }

        // -------------------------------------------------------
        // top — peek at top element without removing
        // -------------------------------------------------------
        Ty top() const
        {
            Node *current = head.load(std::memory_order_acquire);

            if (!current)
                throw std::runtime_error("LockFreeStack is empty!");

            return current->data;
        }

        // -------------------------------------------------------
        // Utility
        // -------------------------------------------------------
        bool empty() const
        {
            return head.load(std::memory_order_acquire) == nullptr;
        }

        unsigned long long size() const
        {
            return _size.load(std::memory_order_relaxed);
        }

        // NOTE: clear() is NOT thread-safe by design —
        // call only when no other threads are using the stack
        void clear()
        {
            Ty ignored;
            while (try_pop(ignored)) {}
        }
    };

} // namespace mini_stl
