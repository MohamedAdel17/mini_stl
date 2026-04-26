#pragma once

#include <iostream>
#include <exception>
using namespace std;
using namespace mini_stl;
using ull = unsigned long long;

namespace mini_stl
{
	template <typename Ty>
	struct NodeD
	{
		Ty data;
		NodeD *prev, *next;

		// Para. const.
		NodeD(NodeD *p, const Ty &val, NodeD *n)
			: data(val), prev(p), next(n) {}
	};

	template <typename Ty>
	class DLL
	{
		using Node = NodeD<Ty>;
		using NodePtr = Node *;

		NodePtr head, tail;
		ull _size;

	public:
		// Dest.
		~DLL() { clear(); }

		// Def. const.
		DLL() : head(nullptr), tail(nullptr), _size(0) {}

		// Copy const.
		DLL(const DLL &right) : head(nullptr), tail(nullptr), _size(0)
		{
			if (!right.empty())
			{

				auto ptr = right.head;

				while (ptr)
				{

					push_back(ptr->data);

					ptr = ptr->next;
				}
			}
		}

		// Assignment operator overloading
		DLL &operator=(const DLL &right)
		{

			clear();

			if (!right.empty())
			{

				auto ptr = right.head;

				while (ptr)
				{

					push_back(ptr->data);

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
				pop_front();
		}

		const Ty &front() const
		{

			if (empty())
				throw exception("Empty double linked list!\n");

			return head->data;
		}
		const Ty &back() const
		{

			if (empty())
				throw exception("Empty double linked list!\n");

			return tail->data;
		}

		// push & pop front

		void push_front(const Ty &val)
		{

			if (empty())
			{

				tail = head = new Node(nullptr, val, nullptr);
				_size++;
				return;
			}

			head = head->prev = new Node(nullptr, val, head);

			_size++;
		}

		void pop_front()
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

		// push & pop back

		void push_back(const Ty &val)
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

		void pop_back()
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

			tail = tail->prev;

			delete tail->next;

			tail->next = nullptr;

			_size--;
		}

		// push & pop at

		void push_at(ull rank, const Ty &val)
		{

			if (rank == _size)
			{
				push_back(val);
				return;
			}
			if (rank == 0)
			{
				push_front(val);
				return;
			}
			if (rank > _size)
				throw out_of_range("Invalid rank!");

			auto ptr = head;

			for (ull i = 1; i <= rank - 1; i++)
				ptr = ptr->next;

			ptr->next = ptr->next->prev = new Node(ptr, val, ptr->next);
			_size++;
		}

		void pop_at(ull rank)
		{

			if (rank == 0)
			{
				pop_front();
				return;
			}
			if (rank == _size - 1)
			{
				pop_back();
				return;
			}
			if (rank >= _size)
				throw out_of_range("Invalid rank!");

			auto ptr = head;

			for (ull i = 1; i <= rank; i++)
				ptr = ptr->next;

			ptr->next->prev = ptr->prev;
			ptr->prev->next = ptr->next;

			delete ptr;
			_size--;
		}

		void swap(DLL &right)
		{
			std::swap(head, right.head);
			std::swap(tail, right.tail);
			std::swap(_size, right._size);
		}

		void remove(const Ty &val)
		{

			while (head && head->data == val)
				pop_front();
			while (tail && tail->data == val)
				pop_back();

			if (empty())
				return;

			auto ptr = head->next;

			while (ptr)
			{

				if (ptr->data == val)
				{

					auto tmp = ptr;

					ptr->next->prev = ptr->prev;
					ptr->prev->next = ptr->next;

					ptr = ptr->next;
					delete tmp;

					_size--;
				}
				else
					ptr = ptr->next;
			}
		}

		void reverse()
		{

			if (head == tail)
				return;

			auto ptr = head;

			while (ptr)
			{

				std::swap(ptr->next, ptr->prev);

				ptr = ptr->prev;
			}

			std::swap(head, tail);
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
			iterator operator++(int)
			{
				auto tmp = *this;
				++*this;
				return tmp;
			}

			iterator &operator--()
			{

				_pointer = _pointer->prev;

				return *this;
			}
			iterator &operator--(int)
			{

				_pointer = _pointer->prev;

				return *this;
			}

			Ty &operator*() const
			{

				return _pointer->data;
			}
		};

		iterator begin() const { return iterator(head); }
		iterator end() const { return iterator(); }

		class reverse_iterator
		{
			NodePtr _pointer;

		public:
			reverse_iterator() : _pointer(nullptr) {}
			reverse_iterator(NodePtr _pointer) : _pointer(_pointer) {}

			bool operator==(const reverse_iterator &right) const
			{
				return (_pointer == right._pointer);
			}
			bool operator!=(const reverse_iterator &right) const
			{
				return !(*this == right);
			}

			reverse_iterator &operator++()
			{

				_pointer = _pointer->prev;

				return *this;
			}
			reverse_iterator &operator++(int)
			{

				_pointer = _pointer->prev;

				return *this;
			}

			iterator &operator--()
			{

				_pointer = _pointer->next;

				return *this;
			}
			reverse_iterator &operator--(int)
			{

				_pointer = _pointer->next;

				return *this;
			}

			Ty &operator*() const
			{

				return _pointer->data;
			}
		};

		reverse_iterator rbegin() const { return reverse_iterator(tail); }
		reverse_iterator rend() const { return reverse_iterator(); }
	};

}
