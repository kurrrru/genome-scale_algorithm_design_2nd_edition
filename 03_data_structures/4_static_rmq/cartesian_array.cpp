#include <iostream>
#include <vector>
#include <string>
#include <stack>

struct cartesian
{
	
	int _root;
	int _n;
	std::vector<int> _parent;
	std::vector<int> _right;
	std::vector<int> _left;
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
};

int main()
{
	std::vector<int> values = {1, 4, 3, 5, 0, 4, 5, 3, 7};
	cartesian tree;
	tree.build(values);
	std::cout << tree.parentheses() << std::endl;
	return (0);
}