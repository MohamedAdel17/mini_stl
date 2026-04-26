#pragma once

#include <mutex>
#include <condition_variable>
#include <stdexcept>

namespace mini_stl
{

    // -------------------------------------------------------
    // ThreadSafeQueue<Ty>
    //
    // A thread-safe FIFO queue backed by a singly-linked list.
    // Multiple producer / multiple consumer safe.
    //
    // Key concurrency primitives used:
    //   - std::mutex          : mutual exclusion for push/pop
    //   - std::condition_variable : lets consumers block and
    //                              wait instead of busy-spinning
    //   - std::unique_lock    : RAII lock that can be released
    //                           temporarily for condition_variable
    // -------------------------------------------------------

    template <typename Ty>
    class ThreadSafeQueue
    {
        // Internal linked-list node
        struct Node
        {
            Ty data;
            Node *next;
            Node(const Ty &val) : data(val), next(nullptr) {}
        };

        Node *head;
        Node *tail;
        unsigned long long _size;

        // Synchronization primitives
        mutable std::mutex _mutex;
        std::condition_variable _cv;

    public:

        // Default constructor
        ThreadSafeQueue() : head(nullptr), tail(nullptr), _size(0) {}

        // Destructor
        ~ThreadSafeQueue() { clear(); }

        // Non-copyable — copying a live mutex is undefined behavior
        ThreadSafeQueue(const ThreadSafeQueue &) = delete;
        ThreadSafeQueue &operator=(const ThreadSafeQueue &) = delete;

        // -------------------------------------------------------
        // push — add element to the back
        // Locks mutex, pushes node, notifies one waiting consumer
        // -------------------------------------------------------
        void push(const Ty &val)
        {
            {
                std::unique_lock<std::mutex> lock(_mutex);

                Node *new_node = new Node(val);

                if (tail)
                    tail->next = new_node;
                else
                    head = new_node;

                tail = new_node;
                _size++;
            } // lock released here

            // Wake up one thread blocked in wait_and_pop()
            _cv.notify_one();
        }

        // -------------------------------------------------------
        // try_pop — non-blocking pop
        // Returns false immediately if queue is empty
        // -------------------------------------------------------
        bool try_pop(Ty &out)
        {
            std::unique_lock<std::mutex> lock(_mutex);

            if (!head)
                return false;

            out = head->data;

            Node *old_head = head;
            head = head->next;

            if (!head)
                tail = nullptr;

            delete old_head;
            _size--;

            return true;
        }

        // -------------------------------------------------------
        // wait_and_pop — blocking pop
        // Sleeps the calling thread until an element is available
        // This is the key advantage over a plain queue in
        // producer/consumer scenarios — zero CPU wasted spinning
        // -------------------------------------------------------
        void wait_and_pop(Ty &out)
        {
            std::unique_lock<std::mutex> lock(_mutex);

            // Lambda predicate: re-check after spurious wakeups
            _cv.wait(lock, [this] { return head != nullptr; });

            out = head->data;

            Node *old_head = head;
            head = head->next;

            if (!head)
                tail = nullptr;

            delete old_head;
            _size--;
        }

        // -------------------------------------------------------
        // front — peek without removing (non-blocking)
        // -------------------------------------------------------
        Ty front() const
        {
            std::unique_lock<std::mutex> lock(_mutex);

            if (!head)
                throw std::runtime_error("ThreadSafeQueue is empty!");

            return head->data;
        }

        // -------------------------------------------------------
        // Utility
        // -------------------------------------------------------
        bool empty() const
        {
            std::unique_lock<std::mutex> lock(_mutex);
            return head == nullptr;
        }

        unsigned long long size() const
        {
            std::unique_lock<std::mutex> lock(_mutex);
            return _size;
        }

        void clear()
        {
            std::unique_lock<std::mutex> lock(_mutex);

            while (head)
            {
                Node *tmp = head;
                head = head->next;
                delete tmp;
            }

            tail = nullptr;
            _size = 0;
        }
    };

} // namespace mini_stl
