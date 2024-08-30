#ifndef CARTEISAN_H
#define CARTEISAN_H

#include <vector>
#include <string>
#include <stack>

struct cartesian
{
	cartesian();
	void build(std::vector<int> &arr);
	std::string parentheses();
	std::vector<int> depth_array();
	std::vector<int> node_array();
	void node_and_idx(std::vector<int> &node, std::vector<int> &node_idx);
private:
	int _root;
	int _n;
	std::vector<int> _parent;
	std::vector<int> _right;
	std::vector<int> _left;
	std::vector<int> _arr;
	std::string parentheses(int cur);
	void depth_array_rec(int cur, std::vector<int> &depth);
	void node_array_rec(int cur, std::vector<int> &node);
	void node_and_idx_rec(int cur, std::vector<int> &node, std::vector<int> &node_idx);
};

#endif
