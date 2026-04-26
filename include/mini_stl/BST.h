#pragma once

#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include <algorithm>
using namespace std;

using namespace mini_stl;

namespace mini_stl
{

	template <typename Ty>
	struct NodeBST
	{
		Ty data;
		NodeBST *left, *right;

		NodeBST(const Ty &data)
			: data(data), left(nullptr), right(nullptr)
		{
		}
	};

	template <typename Ty>
	class BST
	{
		using Node = NodeBST<Ty>;
		using NodePtr = Node *;

		NodePtr root;

		void dfs(NodePtr current_node)
		{

			if (current_node == nullptr)
				return;

			dfs(current_node->left);
			cout << current_node->data << " ";
			dfs(current_node->right);
		}

		NodePtr dfs_copying(NodePtr current_node)
		{

			if (current_node == nullptr)
				return nullptr;

			NodePtr new_node = new Node(current_node->data);

			new_node->left = dfs_copying(current_node->left);
			new_node->right = dfs_copying(current_node->right);

			return new_node;
		}

		NodePtr clear(NodePtr current_node)
		{

			if (current_node == nullptr)
				return nullptr;

			current_node->left = clear(current_node->left);
			current_node->right = clear(current_node->right);
			delete current_node;

			return nullptr;
		}

		void copy_with_bfs(NodePtr start_node)
		{

			if (start_node == nullptr)
				return;

			root = new Node(start_node->data);

			queue<NodePtr> q;
			queue<NodePtr> new_tree;

			q.push(start_node);
			new_tree.push(root);

			while (!q.empty())
			{

				auto node = q.front();
				auto new_tree_node = new_tree.front();

				q.pop();
				new_tree.pop();

				if (node->left)
				{
					q.push(node->left);
					new_tree_node->left = new Node(node->left->data);
					new_tree.push(new_tree_node->left);
				}
				if (node->right)
				{
					q.push(node->right);
					new_tree_node->right = new Node(node->right->data);
					new_tree.push(new_tree_node->right);
				}
			}
		}

	public:
		class bfs_iterator
		{
		public:
			NodePtr _pointer;
			queue<NodePtr> q;

			bfs_iterator() : _pointer(nullptr) {}
			bfs_iterator(NodePtr _pointer) : _pointer(_pointer) {}

			const Ty &operator*() const { return _pointer->data; }

			bfs_iterator &operator++()
			{

				if (q.empty())
				{
					_pointer = nullptr;
					return *this;
				}

				if (q.front()->left)
					q.push(q.front()->left);
				if (q.front()->right)
					q.push(q.front()->right);

				q.pop();

				_pointer = (q.empty() ? nullptr : q.front());

				return *this;
			}

			bfs_iterator &operator++(int)
			{

				if (q.empty())
				{
					_pointer = nullptr;
					return *this;
				}

				if (q.front()->left)
					q.push(q.front()->left);
				if (q.front()->right)
					q.push(q.front()->right);

				q.pop();

				_pointer = (q.empty() ? nullptr : q.front());

				return *this;
			}

			bool operator==(const bfs_iterator &right) const { return (_pointer == right._pointer); }
			bool operator!=(const bfs_iterator &right) const { return !(*this == right); }
		};

		bfs_iterator bfs_begin()
		{

			if (empty())
				return bfs_iterator();
			bfs_iterator itr;

			itr.q.push(root);

			itr._pointer = itr.q.front();

			return itr;
		}
		bfs_iterator bfs_end() const { return bfs_iterator(); }

		class iterator
		{
		public:
			NodePtr _pointer;
			stack<NodePtr> _stack;
			iterator() : _pointer(nullptr) {}
			iterator(NodePtr _pointer) : _pointer(_pointer) {}

			const Ty &operator*() const { return _pointer->data; }

			iterator &operator++()
			{

				if (_stack.empty())
				{
					_pointer = nullptr;
					return *this;
				}

				auto top = _stack.top();
				_stack.pop();

				push_left_side(top->right);

				_pointer = (_stack.empty() ? nullptr : _stack.top());

				return *this;
			}

			iterator operator++(int)
			{
				auto tmp = *this;

				++*this;

				return tmp;
			}

			void push_left_side(NodePtr node)
			{

				if (!node)
					return;

				_stack.push(node);

				auto tmp = node;

				while (tmp->left)
				{
					_stack.push(tmp->left);
					tmp = tmp->left;
				}
			}

			// operator== first to ensure logical consistency
			bool operator==(const iterator &right) const { return (_pointer == right._pointer); }
			bool operator!=(const iterator &right) const { return !(*this == right); }
		};

		iterator begin()
		{

			if (empty())
				return iterator();
			iterator itr;

			auto ptr = root;

			while (ptr)
			{

				itr._stack.push(ptr);

				ptr = ptr->left;
			}

			itr._pointer = itr._stack.top();

			return itr;
		}
		iterator end() { return iterator(); }

		// Def. const.
		BST() : root(nullptr) {}

		// Dest.
		~BST() { clear(); }

		// Copy const.
		BST(const BST &right) : root(nullptr)
		{
			root = dfs_copying(right.root);
		}

		// Operator=
		const BST &operator=(const BST &right)
		{
			clear();

			copy_with_bfs(right.root);

			return *this;
		}

		bool empty() const { return (root == nullptr); }

		void clear()
		{
			root = clear(root);
		}

		pair<iterator, bool> insert(const Ty &val)
		{

			if (empty())
			{

				root = new Node(val);
				return {iterator(root), true};
			}

			auto ptr = root;

			while (val != ptr->data)
			{

				// Look right
				if (val > ptr->data)
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

		iterator find(const Ty &val)
		{

			if (empty())
				return iterator();

			int steps = 0;
			auto ptr = root;

			while (ptr)
			{

				if (val > ptr->data)
					ptr = ptr->right;

				else if (val < ptr->data)
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

		void erase(Ty val)
		{

			if (empty())
				return;

			auto ptr = root;
			auto parent = root;

			while (true)
			{

				if (val > ptr->data)
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

				else if (val < ptr->data)
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
				// ptr is holding the node to be deleted
				else
				{

					// case 1&2: leaf & one child
					if (!ptr->right || !ptr->left)
					{
						// root case
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
						// Fetch smallest big
						auto tmp = ptr->right, tmp_parent = ptr->right;
						while (tmp->left)
						{
							tmp_parent = tmp;
							tmp = tmp->left;
						}

						// handle left side and right side (if needed)
						if (tmp_parent != tmp)
						{
							tmp_parent->left = tmp->right;
							tmp->left = ptr->left;
							tmp->right = ptr->right;
						}
						else
							tmp->left = ptr->left;

						if (root == ptr)
							root = tmp;
						else if (parent->left == ptr)
							parent->left = tmp;
						else
							parent->right = tmp;

						delete ptr;
						return;
					}
				}
			}
		}

		//------------------- Balancing Assignment -------------------//

		// O(n)
		void extract_dfs_in_order(NodePtr current_node, vector<Ty> &v)
		{

			if (current_node == nullptr)
				return;

			extract_dfs_in_order(current_node->left, v);
			v.push_back(current_node->data);
			extract_dfs_in_order(current_node->right, v);
		}

		// O(n log n) because of the sorting, but avoids _stack overflow in case of
		// large linear BST
		void extract_bfs_in_order(vector<Ty> &v)
		{

			queue<NodePtr> q;
			q.push(root);

			while (!q.empty())
			{
				v.push_back(q.front()->data);

				if (q.front()->left)
					q.push(q.front()->left);
				if (q.front()->right)
					q.push(q.front()->right);
				q.pop();
			}

			sort(v.begin(), v.end());
		}

		// O(n)
		NodePtr balanced_insertion(vector<Ty> &v, int left, int right)
		{

			if (right < left)
				return nullptr;

			int mid = (left + right) / 2;

			NodePtr new_node = new Node(v[mid]);

			new_node->left = balanced_insertion(v, left, mid - 1);
			new_node->right = balanced_insertion(v, mid + 1, right);

			return new_node;
		}

		// O(n)
		void balance()
		{

			// Extract the BST into a vector (in-order)
			vector<Ty> v;
			extract_dfs_in_order(root, v); // O(n)

			// clear the bst
			clear(); // O(n)

			// reinsert the values in a proper manner
			root = balanced_insertion(v, 0, v.size() - 1); // O(n)
		}
		//------------------- Balancing -------------------//
	};

}