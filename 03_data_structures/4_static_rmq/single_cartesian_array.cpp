#include <iostream>
#include <vector>
#include <string>
#include <stack>

struct cartesian
{
	cartesian() = default;
	void build(std::vector<int> &arr)
	{
		_root = -1;
		_n = arr.size();
		_parent.assign(_n, -1);
		_right.assign(_n, -1);
		_left.assign(_n, -1);
		_arr = arr;
		std::stack<int> st;
		for (int i = 0; i < _n; i++)
		{
			if (_root == -1)
			{
				_root = i;
				st.push(i);
				continue;
			}
			while (!st.empty() && arr[i] < arr[st.top()])
			{
				st.pop();
			}
			if (st.empty())
			{
				_left[i] = _root;
				_parent[_root] = i;
				_root = i;
				st.push(i);
			}
			else
			{
				_left[i] = _right[st.top()];
				_parent[st.top()] = i;
				_right[st.top()] = i;
				_parent[i] = st.top();
				st.push(i);
			}
		}
	}

	std::string parentheses()
	{
		return (parentheses(_root));
	}

	std::vector<int> depth_array()
	{
		std::vector<int> depth;
		depth.push_back(-1);
		depth_array_rec(_root, depth);
		return (std::vector<int>(depth.begin() + 1, depth.end() - 1));
	}

	std::vector<int> node_array()
	{
		std::vector<int> node;
		node_array_rec(_root, node);
		return (node);
	}

	void node_and_idx(std::vector<int> &node, std::vector<int> &node_idx)
	{
		node_idx.assign(_n, -1);
		node_and_idx_rec(_root, node, node_idx);
	}

private:
	int _root;
	int _n;
	std::vector<int> _parent;
	std::vector<int> _right;
	std::vector<int> _left;
	std::vector<int> _arr;

	std::string parentheses(int cur)
	{
		if (cur == -1)
			return ("");
		std::string result;
		result += "(";
		result += parentheses(_left[cur]);
		result += "()";
		result += parentheses(_right[cur]);
		result += ")";
		return (result);
	}

	void depth_array_rec(int cur, std::vector<int> &depth)
	{
		if (cur == -1)
			return ;
		depth.push_back(depth.end()[-1] + 1);
		depth_array_rec(_left[cur], depth);
		depth_array_rec(_right[cur], depth);
		depth.push_back(depth.end()[-1] - 1);
	}

	void node_array_rec(int cur, std::vector<int> &node)
	{
		if (cur == -1)
			return ;
		if (_left[cur] != -1)
			node.push_back(_arr[cur]);
		node_array_rec(_left[cur], node);
		node.push_back(_arr[cur]);
		node_array_rec(_right[cur], node);
		if (_right[cur] != -1)
			node.push_back(_arr[cur]);
	}

	void node_and_idx_rec(int cur, std::vector<int> &node, std::vector<int> &node_idx)
	{
		if (cur == -1)
			return ;
		if (_left[cur] != -1)
			node.push_back(_arr[cur]);
		node_and_idx_rec(_left[cur], node, node_idx);
		node_idx[cur] = node.size();
		node.push_back(_arr[cur]);
		node_and_idx_rec(_right[cur], node, node_idx);
		if (_right[cur] != -1)
			node.push_back(_arr[cur]);
	}
};

// int main()
// {
// 	std::vector<int> values = {7, 2, 4, 3, 0, 6, 8, 1, 5};
// 	cartesian tree;
// 	tree.build(values);
// 	std::cout << tree.parentheses() << std::endl;
// 	std::vector<int> depth = tree.depth_array();
// 	for (int i = 0; i < depth.size(); i++)
// 	{
// 		std::cout << depth[i] << " ";
// 	}
// 	std::cout << std::endl;
// 	std::vector<int> node = tree.node_array();
// 	for (int i = 0; i < node.size(); i++)
// 	{
// 		std::cout << node[i] << " ";
// 	}
// 	std::cout << std::endl;
// 	return (0);
// }
