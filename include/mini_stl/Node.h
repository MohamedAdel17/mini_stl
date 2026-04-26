#pragma once

namespace mini_stl
{

	template <typename Ty>
	struct Node
	{
		Ty data;
		Node *next;

		// Para. const.
		Node(const Ty &data, Node *next)
			: data(data), next(next)
		{
		}
	};

}
