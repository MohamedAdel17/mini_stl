#pragma once

#include <iostream>
#include <queue>
#include <stack>
using namespace std;
using namespace mini_stl;

namespace mini_stl
{

	template <typename Ty>
	class NodeAVL
	{
	public:
		Ty value;
		int height;
		NodeAVL *left, *right;

		// Parameterized constructor
		NodeAVL(Ty value)
			: value(value), height(0), left(nullptr), right(nullptr)
		{
		}
	};

	template <typename Ty>
	class AVL
	{
		using Node = NodeAVL<Ty>;
		using NodePtr = Node *;

		NodePtr root;

		pair<NodePtr, bool> insert(const Ty &value, NodePtr ptr, NodePtr &insertedNode, NodePtr &duplicate)
		{

			pair<NodePtr, bool> result;

			if (ptr == nullptr)
			{

				result.second = true;
				insertedNode = new Node(value);

				return {insertedNode, true};
			}

			if (value > ptr->value)
			{

				result = insert(value, ptr->right, insertedNode, duplicate);

				ptr->right = result.first;
			}
			else if (value < ptr->value)
			{

				result = insert(value, ptr->left, insertedNode, duplicate);

				ptr->left = result.first;
			}
			else
			{
				// duplicate value

				result.second = false;
				duplicate = ptr;
			}

			// unwinding / backtracking:

			heightUpdate(ptr);

			int bf = balanceFactor(ptr);

			// left left case
			if (bf == 2 && value < ptr->left->value)
			{

				ptr = rightRotation(ptr);
			}

			// left right case
			else if (bf == 2 && value > ptr->left->value)
			{

				ptr->left = leftRotation(ptr->left);

				ptr = rightRotation(ptr);
			}

			// right right case
			else if (bf == -2 && value > ptr->right->value)
			{

				ptr = leftRotation(ptr);
			}

			// right left case
			else if (bf == -2 && value < ptr->right->value)
			{

				ptr->right = rightRotation(ptr->right);

				ptr = leftRotation(ptr);
			}

			return {ptr, result.second};
		}

		NodePtr erase(Ty value, NodePtr ptr)
		{

			if (ptr == nullptr)
				return nullptr;

			if (value > ptr->value)
			{

				ptr->right = erase(value, ptr->right);
			}
			else if (value < ptr->value)
			{

				ptr->left = erase(value, ptr->left);
			}
			else
			{

				// case 1&2: leaf or one child
				if (ptr->right == nullptr || ptr->left == nullptr)
				{

					if (root == ptr)
					{

						root = (root->left ? root->left : root->right);

						delete ptr;

						return root;
					}

					auto tmp = (ptr->right ? ptr->right : ptr->left);

					delete ptr;

					return tmp;
				}
				// case 3: two child
				else
				{

					auto smallest = ptr->right;
					NodePtr smallest_parent = ptr;
					while (smallest->left)
					{
						smallest_parent = smallest;
						smallest = smallest->left;
					}
					// remove smallest from tree if it's not ptr->right
					if (smallest_parent != ptr)
					{
						if (smallest_parent->left == smallest)
							smallest_parent->left = smallest->right;
						else
							smallest_parent->right = smallest->right;
					}
					// set smallest's left to ptr's left
					smallest->left = ptr->left;
					// smallest->right is already correct
					delete ptr;
					return smallest;
				}
			}

			// unwinding / backtracking

			heightUpdate(ptr);
			int bf = balanceFactor(ptr);

			// left left case
			if (bf == 2 && balanceFactor(ptr->left) >= 0)
			{

				ptr = rightRotation(ptr);
			}

			// left right case
			else if (bf == 2 && balanceFactor(ptr->left) < 0)
			{

				ptr->left = leftRotation(ptr->left);

				ptr = rightRotation(ptr);
			}

			// right right case
			else if (bf == -2 && balanceFactor(ptr->right) <= 0)
			{

				ptr = leftRotation(ptr);
			}

			// right left case
			else if (bf == -2 && balanceFactor(ptr->right) > 0)
			{

				ptr->right = rightRotation(ptr->right);

				ptr = leftRotation(ptr);
			}

			return ptr;
		}

		void heightUpdate(NodePtr ptr)
		{

			ptr->height = maxHeight(ptr) + 1;
		}

		int maxHeight(NodePtr ptr)
		{

			return max(getHeight(ptr->left), getHeight(ptr->right));
		}

		int getHeight(NodePtr ptr)
		{

			if (ptr == nullptr)
				return -1;

			return ptr->height;
		}

		int balanceFactor(NodePtr ptr)
		{

			return (getHeight(ptr->left) - getHeight(ptr->right));
		}

		NodePtr rightRotation(NodePtr ptr)
		{

			if (ptr == nullptr || ptr->left == nullptr)
				return nullptr;

			auto leftSide = ptr->left;

			ptr->left = leftSide->right;

			leftSide->right = ptr;

			heightUpdate(ptr);
			heightUpdate(leftSide);

			return leftSide;
		}

		NodePtr leftRotation(NodePtr ptr)
		{

			if (ptr == nullptr || ptr->right == nullptr)
				return nullptr;

			auto rightSide = ptr->right;

			ptr->right = rightSide->left;

			rightSide->left = ptr;

			heightUpdate(ptr);
			heightUpdate(rightSide);

			return rightSide;
		}

		NodePtr dfs_copying(NodePtr current_node)
		{

			if (current_node == nullptr)
				return nullptr;

			NodePtr new_node = new Node(current_node->value);

			new_node->left = dfs_copying(current_node->left);
			new_node->right = dfs_copying(current_node->right);

			return new_node;
		}

		void copy_with_bfs(NodePtr start_node)
		{

			if (start_node == nullptr)
				return;

			root = new Node(start_node->value);

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
					new_tree_node->left = new Node(node->left->value);
					new_tree.push(new_tree_node->left);
				}
				if (node->right)
				{
					q.push(node->right);
					new_tree_node->right = new Node(node->right->value);
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

			const Ty &operator*() const { return _pointer->value; }

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

			const Ty &operator*() const { return _pointer->value; }

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
		AVL() : root(nullptr)
		{
		}

		~AVL()
		{
			clear();
		}

		// Copy const.
		AVL(const AVL &right) : root(nullptr)
		{
			root = dfs_copying(right.root);
		}

		// Operator=
		const AVL &operator=(const AVL &right)
		{
			clear();

			copy_with_bfs(right.root);

			return *this;
		}

		bool empty() const { return (root == nullptr); }

		void clear()
		{

			while (!empty())
			{

				erase(root->value);
			}
		}

		void erase(Ty value)
		{

			root = erase(value, root);
		}

		pair<iterator, bool> insert(const Ty &val)
		{

			NodePtr insertedNode, duplicate;

			auto result = insert(val, root, insertedNode, duplicate);

			root = result.first;

			if (result.second)
			{

				// successful insertion

				return {iterator(insertedNode), true};
			}
			else
			{

				return {iterator(duplicate), false};
			}
		}

		iterator find(const Ty &val)
		{

			int steps = 0;

			if (empty())
				return iterator();

			auto p = root;

			while (val != p->value)
			{

				// move right
				if (val > p->value && p->right != nullptr)
					p = p->right;

				// move left
				else if (val < p->value && p->left != nullptr)
					p = p->left;

				// value is not found!
				else
				{

					cout << "Steps = " << steps << endl;
					return iterator();
				}

				steps++;
			}

			// value is found!
			// value is equal to p->value

			cout << "Steps = " << steps << endl;
			return iterator(p);
		}

		void displayWithBFS()
		{

			queue<pair<NodePtr, int>> q;

			q.push({root, 0});

			while (!q.empty())
			{

				cout << q.front().first->value << ": " << q.front().second << endl;

				if (q.front().first->left)
					q.push({q.front().first->left, q.front().second + 1});
				if (q.front().first->right)
					q.push({q.front().first->right, q.front().second + 1});

				q.pop();
			}
		}
	};

}
