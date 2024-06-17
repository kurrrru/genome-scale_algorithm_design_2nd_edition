#include <iostream>
#include <vector>
#include <string>

struct cartesian
{
	cartesian() : _root(nullptr) {}
	~cartesian() { clear(_root); }

	void build(const std::vector<int> &values)
	{
		for (int value : values)
			insert(value);
	}

	std::string parentheses()
	{
		return (parentheses(_root));
	}

private:
	struct node
	{
		int _value;
		node *_left;
		node *_right;
		node(int value) : _value(value), _left(nullptr), _right(nullptr) {}
	};
	node *_root;

	void clear(node *current)
	{
		if (!current)
			return;
		clear(current->_left);
		clear(current->_right);
		delete current;
	}

	void insert(int value)
	{
		node *new_node = new node(value);
		node *current = _root;
		node *parent = nullptr;
		while (current && current->_value < value)
		{
			parent = current;
			current = current->_right;
		}
		if (!parent)
			_root = new_node;
		else
			parent->_right = new_node;
		new_node->_left = current;
	}

	std::string parentheses(node *current)
	{
		if (!current)
			return "";
		std::string result;
		result += "(";
		result += parentheses(current->_left);
		result += "()";
		result += parentheses(current->_right);
		result += ")";
		return (result);
	}
};

int main(void)
{
	std::vector<int> values = {1, 4, 3, 5, 0, 4, 5, 3, 7};
	cartesian tree;
	tree.build(values);
	std::cout << tree.parentheses() << std::endl;
	return (0);
}
