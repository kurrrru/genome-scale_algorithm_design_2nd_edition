#include "wavelet_tree.h"

static int bit_width(int x)
{
	int res = 0;
	while (x)
	{
		x >>= 1;
		res++;
	}
	return (res);
}

wavelet_tree::_node::_node(int l, int r, std::vector<bool> &arr)
	: _l(l), _r(r), _bv(arr), left(nullptr), right(nullptr) {}

wavelet_tree::wavelet_tree() = default;

wavelet_tree::wavelet_tree(std::vector<int> &arr)
{
	build(arr);
}

// wavelet_tree::build
// @ arr: input array
// assumption: min(arr) = 0
// Build a wavelet tree in O(n log s) time (s = _size, n = _len)
// return: void
void wavelet_tree::build(std::vector<int> &arr)
{
	_len = arr.size();
	_size = *std::max_element(arr.begin(), arr.end());
	_capacity = 1 << bit_width(_size);
	_build_rec(&root, arr, 0, _capacity);
}

// wavelet_tree::access
// @ i: index
// Access the i-th element in O(log s) time (s = _size)
// return: value of the bit at index i
int wavelet_tree::access(int i)
{
	assert(i < _len);
	_node *cur = root;
	int res = 0;
	while (cur != nullptr && cur->_l < cur->_r - 1)
	{
		if (cur->_bv[i])
		{
			res += (cur->_r - cur->_l) / 2;
			i = cur->_bv.rank(i);
			cur = cur->right;
		}
		else
		{
			i = i - cur->_bv.rank(i);
			cur = cur->left;
		}
	}
	return (res);
}

// wavelet_tree::rank
// @ i: index
// @ c: character
// Count the occurrence of c in [0, i) in O(log s) time (s = _size)
// return: the occurrence of c in [0, i)
int wavelet_tree::rank(int i, int c)
{
	assert(i < _len && c < _size);
	_node *cur = root;
	while (cur->_l < cur->_r - 1)
	{
		int mid = (cur->_r + cur->_l) >> 1;
		if (c >= mid)
		{
			i = cur->_bv.rank(i);
			cur = cur->right;
		}
		else
		{
			i = i - cur->_bv.rank(i);
			cur = cur->left;
		}
		if (cur == nullptr)
			return (0);
	}
	return (i);
}

// wavelet_tree::select
// @ i: index
// @ c: character
// Find the i-th occurrence of c.
// return: the index of the i-th occurrence of c
// int wavelet_tree::select(int i, int c)
// {
// 	// not implemented yet
// }

// wavelet_tree::range_count
// @ i: index (inclusive)
// @ j: index (exclusive)
// @ lower: lower bound of the range of characters (inclusive)
// @ upper: upper bound of the range of characters (exclusive)
// Count the number of k that satisfies i <= k < j and lower <= T[k] < upper
// return: the number of k that satisfies the condition
int wavelet_tree::range_count(int i, int j, int lower, int upper)
{
	assert(i < j && lower < upper);
	return (_range_count_rec(root, i, j, lower, upper));
}

// wavelet_tree::freq
// @ i: index (inclusive)
// @ j: index (exclusive)
// @ c: character
// Count the number of k that satisfies i <= k < j and T[k] = c
// return: the number of k that satisfies the condition
int wavelet_tree::freq(int i, int j, int c)
{
	return (rank(j, c) - rank(i, c));
}

// wavelet_tree::is_range_unary
// @ i: index (inclusive)
// @ j: index (exclusive)
// Check if the range [i, j) is unary
// return: true if the range is unary, false otherwise
bool wavelet_tree::is_range_unary(int i, int j)
{
	int c = access(i);
	return (freq(i, j, c) == j - i);
}



void wavelet_tree::_build_rec(_node **cur, std::vector<int> &arr, int lower, int upper)
{
	std::vector<bool> bit(arr.size());
	std::vector<int> left_arr, right_arr;
	if (lower >= _size || upper == lower)
		return ;
	int mid = (lower + upper) >> 1;
	for (int i = 0; i < (int)arr.size(); i++)
	{
		bit[i] = (arr[i] >= mid);
		if (bit[i])
			right_arr.push_back(arr[i]);
		else
			left_arr.push_back(arr[i]);
	}
	*cur = new _node(lower, upper, bit);
	if (lower == upper - 1)
		return ;
	_build_rec(&((*cur)->left), left_arr, lower, mid);
	_build_rec(&((*cur)->right), right_arr, mid, upper);
}

int wavelet_tree::_range_count_rec(_node *cur, int i, int j, int lower, int upper)
{
	if (cur == nullptr || cur->_l >= cur->_r - 1 || i >= j || upper <= cur->_l || lower >= cur->_r)
		return (0);
	if (lower <= cur->_l && cur->_r <= upper)
		return (cur->_bv.rank(j) - cur->_bv.rank(i));
	return (_range_count_rec(cur->left, cur->_bv.rank(i), cur->_bv.rank(j), lower, upper) +
			_range_count_rec(cur->right, i - cur->_bv.rank(i), j - cur->_bv.rank(j), lower, upper));
}