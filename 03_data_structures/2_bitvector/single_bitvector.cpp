/*
bitvector.cpp

This is a simple implementation of a bitvector data structure. 
It supports the following operations:
- build: build a bitvector from an input array
- operator[]: get the value of the bit at index i
- rank: count the occurrence of 1 in [0, i)
- select: get the index of the j-th occurrence of 1

The required space is n + o(n) theoretically, but it is n + O(n) in this implementation.
Also, the select operation can be done in constant time in theory,
but it is O(log n) in this implementation.
*/

#include <iostream>
#include <vector>
#include <cassert>

static unsigned int popcount(unsigned int x)
{
	x = (x & 0b01010101010101010101010101010101) + ((x >> 1) & 0b01010101010101010101010101010101);
	x = (x & 0b00110011001100110011001100110011) + ((x >> 2) & 0b00110011001100110011001100110011);
	x = (x & 0b00001111000011110000111100001111) + ((x >> 4) & 0b00001111000011110000111100001111);
	x = (x & 0b00000000111111110000000011111111) + ((x >> 8) & 0b00000000111111110000000011111111);
	x = (x & 0b00000000000000001111111111111111) + ((x >> 16) & 0b00000000000000001111111111111111);
	return (x);
}

struct bitvector {
	bitvector() = default;
	bitvector(std::vector<bool> &arr) { build(arr); }

	// build
	// @ arr: input array
	// return: void
	void build(std::vector<bool> &arr)
	{
		_len = arr.size();
		_num_blocks = (_len + 0b11111) >> 5;
		_bit.assign(_num_blocks, 0u);
		_sum.assign(_num_blocks + 1, 0u);
		_set(arr);
		for (std::size_t i = 1; i <= _num_blocks; i++)
			_sum[i] = _sum[i - 1] + popcount(_bit[i - 1]);
	}

	// operator[]
	// @ i: index
	// return: value of the bit at index i
	bool operator[](int i)
	{
		assert(i < _len);
		return (_bit[i >> 5] & (1u << (i & 0b11111)));
	}

	// rank
	// @ i: index
	// Count the occurrence of 1 in [0, i) in O(1) time
	// O(n) bits are used in this implementation, but it can be reduced to n + o(n) bits
	// return: the occurrence of 1 in [0, i)
	int rank(int i)
	{
		assert(i <= _len);
		if (i == 0)
			return (0);
		return (_sum[i >> 5] + popcount(_bit[i >> 5] & ((1u << (i & 0b11111)) - 1)));
	}

	// rank
	// @ i: index
	// @ b: the bit to count
	// Count the occurrence of b in [0, i) in O(1) time
	// O(n) bits are used in this implementation, but it can be reduced to n + o(n) bits
	// return: the occurrence of b in [0, i)
	int rank(int i, bool b)
	{
		assert(i <= _len);
		if (i == 0)
			return (0);
		if (b)
			return (rank(i));
		return (i - rank(i));
	}

	// select
	// @ j: the j-th occurrence of 1
	// Ideally, time complexity is O(1), but it is O(log n) in this implementation 
	// return: the index of the j-th occurrence of 1
	int select(int j)
	{
		assert(0 < j && j <= _sum[_num_blocks]);
		int left = -1, right = _len;
		while (left + 1 < right)
		{
			int mid = (left + right) >> 1;
			if (rank(mid) < j)
				left = mid;
			else
				right = mid;
		}
		return (left);
	}

	// select
	// @ j: the j-th occurrence of b
	// @ b: the bit to find
	// Ideally, time complexity is O(1), but it is O(log n) in this implementation
	// return: the index of the j-th occurrence of b
	int select(int j, bool b)
	{
		assert(0 < j && j <= rank(_len, b));
		if (b)
			return (select(j));
		int left = -1, right = _len;
		while (left + 1 < right)
		{
			int mid = (left + right) >> 1;
			if (rank(mid, b) < j)
				left = mid;
			else
				right = mid;
		}
		return (left);
	}

private:
	std::vector<unsigned int> _bit;
	std::vector<unsigned int> _sum;
	int _num_blocks;
	int _len;

	void	_set(std::vector<bool> &arr)
	{
		for (int i = 0; i < _len; i++)
			if (arr[i])
				_bit[i >> 5] |= 1u << (i & 0b11111);
	}
};
