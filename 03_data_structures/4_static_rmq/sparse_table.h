#ifndef SPARSE_TABLE_H
#define SPARSE_TABLE_H

#include <vector>
#include <cassert>

struct sparse_table
{
	sparse_table();
	sparse_table(sparse_table &st);
	sparse_table(std::vector<int> &arr);
	int staticRMQ(int i, int j);
private:
	std::vector<std::vector<int> > _table;
	std::vector<int> _log_table;
	int _n;
	int _log_n;
};


#endif
