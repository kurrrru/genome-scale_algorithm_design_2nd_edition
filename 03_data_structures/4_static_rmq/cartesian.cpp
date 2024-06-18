#include "cartesian.h"
#include <iostream>

cartesian::cartesian() = default;

void cartesian::build(std::vector<int> &arr)
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

std::string cartesian::parentheses()
{
	return (parentheses(_root));
}

std::vector<int> cartesian::depth_array()
{
	std::vector<int> depth;
	depth.push_back(-1);
	depth_array_rec(_root, depth);
	return (std::vector<int>(depth.begin() + 1, depth.end() - 1));
}

std::vector<int> cartesian::node_array()
{
	std::vector<int> node;
	node_array_rec(_root, node);
	return (node);
}

void cartesian::node_and_idx(std::vector<int> &node, std::vector<int> &node_idx)
{
	node_idx.assign(_n, -1);
	node_and_idx_rec(_root, node, node_idx);
}

std::string cartesian::parentheses(int cur)
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

void cartesian::depth_array_rec(int cur, std::vector<int> &depth)
{
	if (cur == -1)
		return ;
	depth.push_back(depth.end()[-1] + 1);
	depth_array_rec(_left[cur], depth);
	depth_array_rec(_right[cur], depth);
	depth.push_back(depth.end()[-1] - 1);
}

void cartesian::node_array_rec(int cur, std::vector<int> &node)
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

void cartesian::node_and_idx_rec(int cur, std::vector<int> &node, std::vector<int> &node_idx)
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
