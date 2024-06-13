#ifndef WAVELET_TREE_H
#define WAVELET_TREE_H

#include "bitvector.h"
#include <vector>
#include <algorithm>
#include <cassert>

struct wavelet_tree
{
	wavelet_tree();
	wavelet_tree(std::vector<int> &arr);
	void build(std::vector<int> &arr);
	int access(int i);
	int rank(int i, int c);
	// int select(int i, int c);
	int range_count(int i, int j, int lower, int upper);
	// std::vector<int> range_list(int i, int j, int lower, int upper);
	int freq(int i, int j, int c);
	bool is_range_unary(int i, int j);
private:
	struct _node
	{
		int _l, _r;
		bitvector _bv;
		_node *left, *right;
		_node(int l, int r, std::vector<bool> &arr);
	};
	_node *root;
	int _len;
	int _size;
	int _capacity;
	void _build_rec(_node **cur, std::vector<int> &arr, int lower, int upper);
	int _range_count_rec(_node *cur, int i, int j, int lower, int upper);
};

#endif
