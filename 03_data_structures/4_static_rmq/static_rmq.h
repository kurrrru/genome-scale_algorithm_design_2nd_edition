#ifndef STATIC_RMQ_H
#define STATIC_RMQ_H

#include "cartesian.h"
#include "sparse_table.h"
#include <vector>
#include <cassert>

struct static_rmq
{
	static_rmq(std::vector<int> &arr);
	int staticRMQ(int l, int r);
private:
	std::vector<int> _arr;
	cartesian _cartesian;
	std::vector<int> _node;
	std::vector<int> _node_idx;
	std::vector<int> _depth;
	std::vector<int> _diff_adjacent;
	std::vector<std::vector<int> > _russian_table;
	std::vector<int> _c;
	sparse_table _c_sparse_table;
	int _n;
	int _x;
	int _k;
	int _arr_size;

	int RMQ_c(int l, int r);
	void fill_russian_table();
};

#endif
