#pragma once

#include <iostream>
#include <exception>
using namespace std;

using ull = unsigned long long;

namespace mini_stl
{

	template <typename Ty>
	class Vector
	{
		// Main components (Attributes)

		ull _size,	   // Number of elements.
			_capacity; // Number of available places.

		Ty *ptr; // Points at our main array. (consider it the array name)

		// O(n)
		void expand()
		{

			_capacity *= 2;

			Ty *tmp = new Ty[_capacity]{};

			for (ull i = 0; i < _size; i++)
			{

				tmp[i] = ptr[i];
			}

			delete[] ptr;

			ptr = tmp;
		}

		void start_vector(const Ty &val)
		{

			_capacity = 1;
			_size = 1;

			ptr = new Ty[1]{val};
		}

		void end_vector()
		{
			delete[] ptr;
			ptr = nullptr;
			_capacity = _size = 0;
		}

	public:
		// Dest.
		~Vector() { delete[] ptr; }
		// Def. const.
		Vector() : _size(0), _capacity(0), ptr(nullptr) {}

		// Para. const.
		Vector(ull _capacity)
			: _capacity(_capacity), _size(_capacity), ptr(new Ty[_capacity]{})
		{
		}

		Vector(ull _capacity, const Ty &val)
			: _capacity(_capacity), _size(_capacity), ptr(new Ty[_capacity]{})
		{
			for (ull i = 0; i < _size; i++)
				ptr[i] = val;
		}

		// Copy const.
		Vector(const Vector &right)
			: _size(right._size), _capacity(right._size), ptr(nullptr)
		{

			if (right.ptr)
			{

				ptr = new Ty[_size]{};

				for (ull i = 0; i < _size; i++)
					ptr[i] = right.ptr[i];
			}
		}

		// Assignment operator overloading
		Vector &operator=(const Vector &right)
		{

			end_vector();

			_size = right._size;
			_capacity = right._size;

			if (right.ptr)
			{

				ptr = new Ty[_size]{};

				for (ull i = 0; i < _size; i++)
					ptr[i] = right.ptr[i];
			}

			return *this;
		}

		void fill(const Ty &val)
		{
			for (ull i = 0; i < _size; i++)
				ptr[i] = val;
		}

		ull size() const { return _size; }
		ull capacity() const { return _capacity; }
		bool full() const { return (_size == _capacity && _size != 0); }
		bool empty() const { return (_size == 0); }
		void clear() { _size = 0; }

		Ty &operator[](ull index) const
		{

			if (index >= _size)
				throw out_of_range("Invalid index!\n");

			return ptr[index];
		}

		// push & pop back

		// Best case: O(1), Worst case: O(n), Amortized case: O(1)
		void push_back(const Ty &val)
		{

			if (ptr == nullptr)
			{
				start_vector(val);
				return;
			}

			if (full())
				expand(); // O(n)

			ptr[_size++] = val;
		}

		// O(1)
		void pop_back()
		{

			if (empty())
				return;

			_size--;
		}

		// push & pop at --> O(n)
		void push_at(ull index, const Ty &val)
		{

			if (index > _size)
				throw out_of_range("Invalid index!\n");

			if (ptr == nullptr)
			{
				start_vector(val);
				return;
			}

			if (full())
				expand();

			// Shifting loop
			for (ull i = _size; i > index; i--)
			{
				ptr[i] = ptr[i - 1];
			}

			ptr[index] = val;
			_size++;
		}

		void pop_at(ull index)
		{

			if (index >= _size)
				throw out_of_range("Invalid index!\n");

			// Shifting loop
			for (ull i = index; i <= _size - 2; i++)
				ptr[i] = ptr[i + 1];

			pop_back(); // to handle the duplicate at the end
		}

		// push & pop front --> O(n)
		void push_front(const Ty &val) { push_at(0, val); }
		void pop_front() { pop_at(0); }

		// O(n)
		void shrink_to_fit()
		{

			if (full())
				return;

			if (empty())
			{
				end_vector();
				return;
			}

			_capacity = _size;

			Ty *tmp = new Ty[_capacity]{};

			for (ull i = 0; i < _size; i++)
			{

				tmp[i] = ptr[i];
			}

			delete[] ptr;

			ptr = tmp;
		}

		// O(1)
		void swap(Vector &right)
		{

			std::swap(_size, right._size);
			std::swap(_capacity, right._capacity);
			std::swap(ptr, right.ptr);
		}

		class iterator
		{
			Ty *_pointer;

		public:
			iterator() : _pointer(nullptr) {}
			iterator(Ty *_pointer) : _pointer(_pointer) {}

			bool operator==(const iterator &right) const
			{
				return (_pointer == right._pointer);
			}
			bool operator!=(const iterator &right) const
			{
				return !(*this == right);
			}

			iterator &operator++()
			{ // pre-fix
				_pointer++;
				return *this;
			}
			iterator &operator++(int)
			{ // post-fix
				_pointer++;
				return *this;
			}

			Ty &operator*() const
			{
				return *_pointer;
			}
		};

		iterator begin() const
		{
			return iterator(ptr);
		}
		iterator end() const
		{
			return iterator(ptr + _size);
		}
	};

}
