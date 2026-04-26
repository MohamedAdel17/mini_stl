#pragma once

#include <iostream>
#include <exception>
using namespace std;

using ull = unsigned long long;

namespace mini_stl
{

	template <typename Ty>
	struct NodeS
	{
		Ty data;
		NodeS *next;

		// Para. const.
		NodeS(const Ty &data, NodeS *next)
			: data(data), next(next)
		{
		}
	};

	template <typename Ty>
	class Stack
	{
		using Node = NodeS<Ty>;
		using NodePtr = Node *;

		// Attributes
		NodePtr head;
		ull _size;

	public:
		// Dest.
		~Stack() { clear(); }
		// Def. const.
		Stack() : head(nullptr), _size(0) {}

		// Copy Const. ---> time: O(n), space: O(1)
		Stack(const Stack &right) : _size(0), head(nullptr)
		{

			auto right_ptr = right.head;

			if (!right.empty())
			{

				push(right_ptr->data);

				auto ptr = head;

				right_ptr = right_ptr->next;

				while (right_ptr)
				{

					ptr->next = new Node(right_ptr->data, nullptr);

					ptr = ptr->next;

					right_ptr = right_ptr->next;
				}
				_size = right._size;
			}
		}

		// Assignment operator overloading
		Stack &operator=(const Stack &right)
		{

			clear();

			auto right_ptr = right.head;

			if (!right.empty())
			{

				push(right_ptr->data);

				auto ptr = head;

				right_ptr = right_ptr->next;

				while (right_ptr)
				{

					ptr->next = new Node(right_ptr->data, nullptr);

					ptr = ptr->next;

					right_ptr = right_ptr->next;
				}
				_size = right._size;
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

		Ty &top() const
		{

			if (empty())
				throw runtime_error("Empty Stack!");

			return head->data;
		}

		// push & pop front --> O(1)

		void push(const Ty &val)
		{

			head = new Node(val, head);
			_size++;
		}

		void pop()
		{

			if (empty())
				return;

			auto ptr = head;

			head = head->next;

			delete ptr;

			_size--;
		}

		void swap(Stack &right)
		{

			std::swap(head, right.head);
			std::swap(_size, right._size);
		}
	};

}
