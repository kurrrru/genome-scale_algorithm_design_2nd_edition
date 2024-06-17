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

	std::string parentheses(int cur)
	{
		if (cur == -1 || cur >= _n)
			return ("");
		std::string result;
		result += "(";
		result += parentheses(_left[cur]);
		result += "()";
		result += parentheses(_right[cur]);
		result += ")";
		return (result);
	}

	std::vector<int> depth_array()
	{
		std::vector<int> depth;
		depth.push_back(-1);
		depth_array_rec(_root, depth);
		return (std::vector<int>(depth.begin() + 1, depth.end() - 1));
	}

private:
	int _root;
	int _n;
	std::vector<int> _parent;
	std::vector<int> _right;
	std::vector<int> _left;

	void depth_array_rec(int cur, std::vector<int> &depth)
	{
		if (cur == -1)
			return ;
		depth.push_back(depth.end()[-1] + 1);
		depth_array_rec(_left[cur], depth);
		depth_array_rec(_right[cur], depth);
		depth.push_back(depth.end()[-1] - 1);
	}
};

int main()
{
	std::vector<int> values = {7, 2, 4, 3, 0, 6, 8, 1, 5};
	cartesian tree;
	tree.build(values);
	std::cout << tree.parentheses() << std::endl;
	std::vector<int> depth = tree.depth_array();
	for (int i = 0; i < depth.size(); i++)
	{
		std::cout << depth[i] << " ";
	}
	std::cout << std::endl;
	return (0);
}
