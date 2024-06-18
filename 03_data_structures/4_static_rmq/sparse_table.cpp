#include "sparse_table.h"


sparse_table::sparse_table()
{
}

sparse_table::sparse_table(sparse_table &st)
{
	_table = st._table;
	_log_table = st._log_table;
	_n = st._n;
	_log_n = st._log_n;
}

sparse_table::sparse_table(std::vector<int> &arr)
{
	_n = arr.size();
	_log_table.assign(_n + 1, 0);
	for (int i = 2; i <= _n; i++)
		_log_table[i] = _log_table[i / 2] + 1;
	_log_n = _log_table[_n];
	_table.assign(_n, std::vector<int>(_log_n + 1, 0));
	for (int i = 0; i < _n; i++)
		_table[i][0] = arr[i];
	for (int j = 1; j <= _log_n; j++)
		for (int i = 0; i + (1 << j) <= _n; i++)
			_table[i][j] = std::min(_table[i][j - 1], _table[i + (1 << (j - 1))][j - 1]);
}

int sparse_table::staticRMQ(int i, int j)
{
	int k = _log_table[j - i];
	return (std::min(_table[i][k], _table[j - (1 << k)][k]));
}
