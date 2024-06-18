/*
This is a simple implementation of a data structure for static range minimum query.
The data structure is built in O(n) time and can answer queries in O(1) time.

Attributes:
	_arr: the array that RMQ queries are performed on.
	_cartesian: cartesian tree of _arr. Each node has a index of _arr.
	_node: node array of _cartesian. Each element is a value of _arr.
	_node_idx: node index array of _cartesian.
		The index of _node_idx is the index of _arr, and the value is the index of _node.
	_depth: depth array of _cartesian.
		This array is used to answer RMQ queries on _node, instead of using _node directly.
	_diff_adjacent: difference between adjacent elements in _depth
	_russian_table: table for the four russians method
	_c: array of minimum values of each block of size _x in _node
	_n: size of _depth, _n = _arr_size * 2 - 2
	_x: size of a block in _node
	_k: size of _c
	_arr_size: size of _arr
*/

#include "static_rmq.h"
#include <iostream>

static int popcount(int n)
{
	n = (n & 0x55555555) + ((n >> 1) & 0x55555555);
	n = (n & 0x33333333) + ((n >> 2) & 0x33333333);
	n = (n & 0x0f0f0f0f) + ((n >> 4) & 0x0f0f0f0f);
	n = (n & 0x00ff00ff) + ((n >> 8) & 0x00ff00ff);
	n = (n & 0x0000ffff) + ((n >> 16) & 0x0000ffff);
	return (n);
}

static int bit_width(int n)
{
	n |= n >> 1;
	n |= n >> 2;
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;
	return (popcount(n));
}


// static_rmq
// @ arr: array of integers
// build a data structure for static range minimum query in O(n) time
// use 1 << 30 as a dummy value, so that the range can be [l, arr.size())
// return: none
static_rmq::static_rmq(std::vector<int> &arr)
{
	_arr = arr;
	_arr_size = _arr.size();
	_arr.push_back(1 << 30);
	_cartesian.build(_arr);
	_cartesian.node_and_idx(_node, _node_idx);
	_depth = _cartesian.depth_array();
	_n = _depth.size();
	_x = (bit_width(_n) - 1) / 2 + 1;
	_diff_adjacent.assign(_n, 0);
	int roll = 0;
	for (int i = 0; i < _x; i++)
		roll = (roll << 1) | (_depth[i + 1] - _depth[i] == 1);
	_diff_adjacent[0] = roll;
	for (int i = 1; i < _n - _x; i++)
	{
		roll = (roll << 1) | (_depth[i + _x] - _depth[i + _x - 1] == 1);
		roll &= (1 << _x) - 1;
		_diff_adjacent[i] = roll;
	}
	for (int i = _n - _x; i < _n; i++)
	{
		roll = (roll << 1) | 1;
		roll &= (1 << _x) - 1;
		_diff_adjacent[i] = roll;
	}
	fill_russian_table();
	_k = (_n - 1) / _x + 1;
	_c.assign(_k, 0);
	for (int i = 0; i < _k; i++)
		_c[i] = _node[i * _x + _russian_table[_diff_adjacent[i * _x]][_x - 2]];
	_c_sparse_table = sparse_table(_c);
}

// staticRMQ
// @ l: left index
// @ r: right index
// Calculate the minimum value in [l, r) in O(1) time
// return: minimum value in [l, r)
int static_rmq::staticRMQ(int l, int r)
{
	int l_node = _node_idx[l];
	int r_node = _node_idx[r - 1]; // r - 1にすべきかも
	if (r_node - l_node == 0)
		return (_node[l_node]);
	if (r_node - l_node <= _x)
		return (_node[l_node + _russian_table[_diff_adjacent[l_node]][r_node - l_node - 1]]);
	return (std::min(RMQ_c((l_node - 1) / _x + 1, r_node / _x), std::min(_node[l_node + _russian_table[_diff_adjacent[l_node]][_x - 1]], _node[r_node - _x + _russian_table[_diff_adjacent[r_node - _x]][_x - 1]])));
}

int static_rmq::RMQ_c(int l, int r)
{
	return (_c_sparse_table.staticRMQ(l, r));
}

void static_rmq::fill_russian_table()
{
	_russian_table.assign(1 << _x, std::vector<int>(_x, -1));
	for (int bit = 0; bit < (1 << _x); bit++)
	{
		int min_i = 0;
		int min_val = 0;
		int val = 0;
		for (int i = 0; i < _x; i++)
		{
			if ((bit >> (_x - i - 1)) & 1)
				val++;
			else
				val--;
			if (val < min_val)
			{
				min_val = val;
				min_i = i + 1;
			}
			_russian_table[bit][i] = min_i;
		}
	}
}
