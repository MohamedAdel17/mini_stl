#pragma once

#include <iostream>
#include <exception>
using namespace std;
using namespace mini_stl;

using ull = unsigned long long;

namespace mini_stl
{

	template <typename Ty>
	struct NodeQ
	{
		Ty data;
		NodeQ *prev, *next;

		// Para. const.
		NodeQ(NodeQ *p, const Ty &val, NodeQ *n)
			: data(val), prev(p), next(n) {}
	};

	template <typename Ty>
	class Queue
	{
		using Node = NodeQ<Ty>;
		using NodePtr = Node *;

		NodePtr head, tail;
		ull _size;

	public:
		// Dest.
		~Queue() { clear(); }

		// Def. const.
		Queue() : head(nullptr), tail(nullptr), _size(0) {}

		// Copy const.
		Queue(const Queue &right) : head(nullptr), tail(nullptr), _size(0)
		{
			if (!right.empty())
			{

				auto ptr = right.head;

				while (ptr)
				{

					push(ptr->data);

					ptr = ptr->next;
				}
			}
		}

		// Assignment operator overloading
		Queue &operator=(const Queue &right)
		{

			clear();

			if (!right.empty())
			{

				auto ptr = right.head;

				while (ptr)
				{

					push(ptr->data);

					ptr = ptr->next;
				}
			}
			return *this;
		}

		bool empty() const { return (head == nullptr); }
		ull size() const { return _size; }
		void clear()
		{
			while (!empty())
				pop();
		}

		const Ty &front() const
		{

			if (empty())
				throw runtime_error("Empty Queue!\n");

			return head->data;
		}
		const Ty &back() const
		{

			if (empty())
				throw runtime_error("Empty Queue!\n");

			return tail->data;
		}

		void pop()
		{

			if (empty())
				return;

			if (head == tail)
			{

				delete head;

				head = tail = nullptr;

				_size--;

				return;
			}

			head = head->next;

			delete head->prev;

			head->prev = nullptr;

			_size--;
		}

		void push(const Ty &val)
		{

			if (empty())
			{

				tail = head = new Node(nullptr, val, nullptr);
				_size++;
				return;
			}

			tail = tail->next = new Node(tail, val, nullptr);

			_size++;
		}

		void swap(Queue &right)
		{
			std::swap(head, right.head);
			std::swap(tail, right.tail);
			std::swap(_size, right._size);
		}
	};

}
