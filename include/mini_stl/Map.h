#pragma once

#include <iostream>
using namespace std;
using namespace mini_stl;

namespace mini_stl
{

	template <typename Ty1, typename Ty2>
	struct Map_Node
	{
		pair<const Ty1, Ty2> data;
		Map_Node *left, *right;

		// Para. const.
		Map_Node(const pair<const Ty1, Ty2> &data)
			: data(data), left(nullptr), right(nullptr)
		{
		}
	};

	template <typename Ty1, typename Ty2>
	class Map
	{
		using Node = Map_Node<Ty1, Ty2>;
		using NodePtr = Node *;

		NodePtr root;

	public:
		class iterator
		{
			NodePtr _pointer;

		public:
			iterator() : _pointer(nullptr) {}
			iterator(NodePtr _pointer) : _pointer(_pointer) {}

			bool operator==(const iterator &right) const { return (_pointer == right._pointer); }
			bool operator!=(const iterator &right) const { return !(*this == right); }

			pair<const Ty1, Ty2> &operator*() const { return _pointer->data; }

			pair<const Ty1, Ty2> *operator->() const
			{
				return &(_pointer->data);
			}
		};

		iterator end() const { return iterator(); }

		// Def. const.
		Map() : root(nullptr) {}

		// Dest.
		~Map() { clear(); }

		bool empty() const { return (root == nullptr); }

		void clear()
		{
			while (!empty())
				erase(root->data.first);
		}

		pair<iterator, bool> insert(const pair<Ty1, Ty2> &val)
		{

			if (empty())
			{

				root = new Node(val);
				return {iterator(root), true};
			}

			auto ptr = root;

			while (val.first != ptr->data.first)
			{

				// Look right
				if (val.first > ptr->data.first)
				{

					if (ptr->right == nullptr)
					{

						ptr->right = new Node(val);

						return {iterator(ptr->right), true};
					}
					else
						ptr = ptr->right; // empty right
				}

				// Look left
				else
				{

					if (ptr->left == nullptr)
					{

						ptr->left = new Node(val);

						return {iterator(ptr->left), true};
					}
					else
						ptr = ptr->left; // empty left
				}
			}

			// value is found!
			return {iterator(ptr), false};
		}

		iterator find(const Ty1 &val)
		{

			if (empty())
				return iterator();

			int steps = 0;
			auto ptr = root;

			while (ptr)
			{

				if (val > ptr->data.first)
					ptr = ptr->right;

				else if (val < ptr->data.first)
					ptr = ptr->left;

				else
				{
					cout << "Steps = " << steps << endl;
					return iterator(ptr);
				}

				steps++;
			}

			cout << "Steps = " << steps << endl;
			return iterator();
		}

		void erase(Ty1 val)
		{

			if (empty())
				return;

			auto ptr = root;
			auto parent = root;

			while (true)
			{

				// Greater than
				if (val > ptr->data.first)
				{

					// Look right
					if (ptr->right)
					{
						parent = ptr;
						ptr = ptr->right;
					}
					else
						return; // not found!
				}

				// Less than
				else if (val < ptr->data.first)
				{

					// Look left
					if (ptr->left)
					{
						parent = ptr;
						ptr = ptr->left;
					}
					else
						return; // not found!
				}

				// Value is found!
				// parent is holding the parent node
				// ptr is holding the node to be deleted    ptr is سفاح
				else
				{

					// case 1&2: leaf & one child
					if (!ptr->right || !ptr->left)
					{

						if (ptr == root)
						{

							root = (root->right ? root->right : root->left);

							delete ptr;
							return;
						}

						// tmp points at the non-null child (if existed)
						auto tmp = (ptr->right ? ptr->right : ptr->left);

						if (parent->right == ptr)
							parent->right = tmp;
						else
							parent->left = tmp;

						delete ptr;
						return;
					}

					// case 3: two child
					else
					{

						auto smallestBig = ptr->right;

						while (smallestBig->left)
							smallestBig = smallestBig->left;

						auto tmp = ptr;

						if (root == ptr)
						{

							root = new Node(smallestBig->data);
							root->left = ptr->left;
							root->right = ptr->right;

							parent = root;
							ptr = root->right;

							delete tmp;

							val = smallestBig->data.first;
						}
						else
						{

							if (parent->right == ptr)
							{
								parent->right = new Node(smallestBig->data);
								parent->right->left = ptr->left;
								parent->right->right = ptr->right;
								ptr = parent->right->right;
								parent = parent->right;
							}
							else
							{
								parent->left = new Node(smallestBig->data);
								parent->left->left = ptr->left;
								parent->left->right = ptr->right;
								ptr = parent->left->right;
								parent = parent->left;
							}

							delete tmp;

							val = smallestBig->data.first;
						}
					}
				}
			}
		}

		Ty2 &operator[](const Ty1 &key)
		{

			if (empty())
			{

				Ty2 value{};

				root = new Node({key, value});
				return root->data.second;
			}

			auto ptr = root;

			while (key != ptr->data.first)
			{

				// Look right
				if (key > ptr->data.first)
				{

					if (ptr->right == nullptr)
					{

						Ty2 value{};
						ptr->right = new Node({key, value});

						return ptr->right->data.second;
					}
					else
						ptr = ptr->right; // empty right
				}

				// Look left
				else
				{

					if (ptr->left == nullptr)
					{

						Ty2 value{};
						ptr->left = new Node({key, value});

						return ptr->left->data.second;
					}
					else
						ptr = ptr->left; // empty left
				}
			}

			// value is found!
			return ptr->data.second;
		}
	};

}
