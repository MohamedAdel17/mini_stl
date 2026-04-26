#pragma once

#include <iostream>
#include <exception>
using namespace std;
using namespace mini_stl;

using ull = unsigned long long;

namespace mini_stl
{

	template <typename Ty>
	struct NodeSLL
	{
		Ty data;
		NodeSLL *next;

		// Para. const.
		NodeSLL(const Ty &data, NodeSLL *next)
			: data(data), next(next)
		{
		}
	};

	template <typename Ty>
	class SLL
	{
		using Node = NodeSLL<Ty>;
		using NodePtr = Node *;

		// Attributes
		NodePtr head;
		ull _size;

	public:
		// Dest.
		~SLL() { clear(); }
		// Def. const.
		SLL() : head(nullptr), _size(0) {}

		// Copy Const. ---> time: O(n), space: O(1)
		SLL(const SLL &right) : _size(0), head(nullptr)
		{

			auto right_ptr = right.head;

			if (!right.empty())
			{

				push_front(right_ptr->data);

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
		SLL &operator=(const SLL &right)
		{

			clear();

			auto right_ptr = right.head;

			if (!right.empty())
			{

				push_front(right_ptr->data);

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
				pop_front();
		}

		Ty front() const
		{

			if (empty())
				throw exception("Empty Linked List!\n");

			return head->data;
		}

		// push & pop front --> O(1)

		void push_front(const Ty &val)
		{

			head = new Node(val, head);
			_size++;
		}

		void pop_front()
		{

			if (empty())
				return;

			auto ptr = head;

			head = head->next;

			delete ptr;

			_size--;
		}

		// push & pop back ---> O(n)

		void push_back(const Ty &val)
		{

			if (empty())
			{
				push_front(val);
				return;
			}

			auto ptr = head;

			while (ptr->next)
				ptr = ptr->next;

			// after this loop, ptr is holding the last node
			// let the ptr->next hold the new node instead of nullptr

			ptr->next = new Node(val, nullptr);
			_size++;
		}

		void pop_back()
		{

			if (empty())
				return;

			if (head->next == nullptr)
			{
				pop_front();
				return;
			}

			auto ptr = head;

			while (ptr->next->next)
				ptr = ptr->next;

			// after this loop, ptr is holding the node before last
			// ptr->next is holding the last node (the one to be deleted)

			delete ptr->next;

			// let ptr->next equal nullptr in replace of the deleted node
			ptr->next = nullptr;
			_size--;
		}

		// push & pop at  --> O(n)

		void push_at(ull rank, const Ty &val)
		{

			if (rank > _size)
				throw out_of_range("Invalid Linked List Length!");

			if (rank == 0)
			{
				push_front(val);
				return;
			}

			auto ptr = head;

			for (ull i = 1; i <= (rank - 1); i++)
				ptr = ptr->next;

			ptr->next = new Node(val, ptr->next);
			_size++;
		}

		void pop_at(ull rank)
		{

			if (rank >= _size)
				throw out_of_range("Invalid Linked List Length!");

			if (rank == 0)
			{
				pop_front();
				return;
			}

			auto ptr = head;

			for (ull i = 1; i <= (rank - 1) && ptr->next; i++)
				ptr = ptr->next;

			auto tmp = ptr->next;
			ptr->next = ptr->next->next;
			delete tmp;

			_size--;
		}

		void swap(SLL &right)
		{

			std::swap(head, right.head);
			std::swap(_size, right._size);
		}

		void remove(const Ty &val)
		{

			while (head && head->data == val)
				pop_front();

			if (empty())
				return;

			auto ptr = head;

			while (ptr->next)
			{

				if (ptr->next->data == val)
				{

					auto tmp = ptr->next;

					ptr->next = ptr->next->next;

					delete tmp;

					_size--;
				}
				else
					ptr = ptr->next;
			}
		}

		void reverse()
		{

			if (empty() || head->next == nullptr)
				return;

			auto ptr1 = head;
			auto ptr2 = head->next;
			auto tmp = ptr2->next;

			while (ptr2)
			{
				ptr2->next = ptr1;
				ptr1 = ptr2;
				ptr2 = tmp;
				if (tmp)
					tmp = tmp->next;
			}

			head->next = nullptr;
			head = ptr1;
		}

		class iterator
		{
			NodePtr _pointer;

		public:
			iterator() : _pointer(nullptr) {}
			iterator(NodePtr _pointer) : _pointer(_pointer) {}

			bool operator==(const iterator &right) const
			{
				return (_pointer == right._pointer);
			}
			bool operator!=(const iterator &right) const
			{
				return !(*this == right);
			}

			iterator &operator++()
			{

				_pointer = _pointer->next;

				return *this;
			}
			iterator &operator++(int)
			{

				_pointer = _pointer->next;

				return *this;
			}

			Ty &operator*() const
			{

				return _pointer->data;
			}
		};

		iterator begin() const { return iterator(head); }
		iterator end() const { return iterator(); }
	};

}
