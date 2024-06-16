#ifndef BITVECTOR_H
#define BITVECTOR_H

#include <vector>
#include <cassert>

struct bitvector
{
	bitvector();
	bitvector(std::vector<bool> &arr);
	void build(std::vector<bool> &arr);
	bool operator[](int i);
	int rank(int i);
	int rank(int i, bool b);
	int select(int j);
	int select(int j, bool b);
private:
	std::vector<unsigned int> _bit;
	std::vector<unsigned int> _sum;
	std::size_t _num_blocks;
	std::size_t _len;
	void _set(std::vector<bool> &arr);
};


#endif