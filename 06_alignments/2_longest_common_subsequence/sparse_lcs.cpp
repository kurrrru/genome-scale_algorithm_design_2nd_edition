#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

namespace
{
	int bit_width(int x)
	{
		int res = 0;
		while (x > 0)
		{
			x >>= 1;
			res++;
		}
		return (res);
	}

	template <typename Key, typename Value>
	struct bst_naive
	{
		bst_naive() = default;
		bst_naive(bst_naive const& bst) = default;
		bst_naive& operator=(bst_naive const& bst) = default;

		bst_naive(Value _inf) : root(nullptr), inf(_inf), size(0), capacity(0) {}

		// bst_naive::build
		// @ keys: the keys of the nodes to be built
		// build the semi-dynamic balanced binary search tree with the given keys
		// return: void
		void build(const std::vector<Key> &keys)
		{
			size = keys.size();
			capacity = 1 << bit_width(size);
			build_rec(&root, keys, 0, capacity);
		}

		// bst_naive::rmq
		// @ l: the lower bound of the range
		// @ r: the upper bound of the range
		// find the node with the minimum value among the nodes whose key is in [l, r]
		// return: the value of the node, or inf if the node does not exist
		Value	rmq(const Key l, const Key r) const
		{
			return (rmq_rec(root, l, r));
		}

		// bst_naive::update
		// @ idx: the key of the node to be updated
		// @ val: the new value of the node
		// update the value of the node whose key is idx
		// return: true if the node exists, false otherwise
		bool	update(Key idx, Value val)
		{
			return (update_rec(root, idx, val));
		}

		~bst_naive()
		{
			delete_rec(root);
		}
	private:
		struct node
		{
			Key		key;
			Value	value;
			node	*left;
			node	*right;
			node(Key _key, Value _value) : key(_key), value(_value), left(nullptr), right(nullptr) {}
		};
		node	*root;
		Value	inf;
		int		size;
		int		capacity;

		void build_rec(node **cur, const std::vector<Key> &keys, int l, int r)
		{
			if (l >= size || l == r) return ;
			*cur = new node(keys.at(std::min(r - 1, size - 1)), inf);
			if (r == l + 1) return ;
			int m = (l + r) / 2;
			build_rec(&((*cur)->left), keys, l, m);
			build_rec(&((*cur)->right), keys, m, r);
		}

		Value	rmq_rec(node *cur, const Key l, const Key r) const
		{
			if (cur == nullptr || cur->key < l) return (inf);
			if (cur->left && cur->left->key >= l && cur->right && cur->right->key <= r)
				return (std::min(cur->right->value, rmq_rec(cur->left, l, r)));
			if (cur->left && (cur->left->key >= r || cur->right == nullptr))
				return (rmq_rec(cur->left, l, r));
			if (cur->left && cur->left->key < l)
				return (rmq_rec(cur->right, l, r));
			if (cur->left && cur->right)
				return (std::min(rmq_rec(cur->left, l, r), rmq_rec(cur->right, l, r)));
			return (cur->value);
		}

		bool	update_rec(node *cur, Key idx, Value val)
		{
			if (cur == nullptr || cur->key < idx) return (false);
			bool res = false;
			if (cur->key > idx)
				res = update_rec(cur->left, idx, val) || update_rec(cur->right, idx, val);
			else if (cur->key == idx && cur->right != nullptr)
				res = update_rec(cur->right, idx, val);
			else
			{
				cur->value = val;
				return (true);
			}
			if (res && cur->left && cur->right)
				cur->value = std::min(cur->left->value, cur->right->value);
			else if (res && cur->left)
				cur->value = cur->left->value;
			return (res);
		}

		void	delete_rec(node *cur)
		{
			if (cur == nullptr) return ;
			delete_rec(cur->left);
			delete_rec(cur->right);
			delete cur;
		}
	};
}

// sparse_edit_distance
// @ s1: the first string
// @ s2: the second string
// compute the edit distance between s1 and s2 using the sparse dynamic programming approach
// time: O((m + n + |M|)log m)
// space: O(m + |M|)
// |M| is the number of pairs (i, j) such that s1[i] = s2[j]
// return: the edit distance between s1 and s2
int sparse_edit_distance(const std::string &s1, const std::string &s2)
{
	int m = s1.size();
	int n = s2.size();
	std::vector<std::vector<int> > ch(256);
	std::vector<std::pair<int, int> > M;
	for (int i = m - 1; i >= 0; i--)
		ch[(int)s1[i]].push_back(i);
	for (int j = 0; j < n; j++)
		for (int i : ch[(int)s2[j]])
			M.push_back(std::make_pair(i, j));
	std::vector<int> arr(m + 1, 0);
	for (int i = 0; i < m + 1; i++)
		arr[i] = i;
	bst_naive<int, int> bst(1e9);
	bst.build(arr);
	bst.update(0, 0);
	for (int p = 0; p < (int)M.size(); p++)
	{
		auto [i, j] = M[p];
		int d = i + j + bst.rmq(0, i);
		bst.update(i + 1, d - i - j - 2);
	}
	return (bst.rmq(0, m) + m + n);
}

// sparse_LCS
// @ s1: the first string
// @ s2: the second string
// compute the length of the longest common subsequence between s1 and s2
//   using the sparse dynamic programming approach on the edit distance
// time: O((m + n + |M|)log m)
// space: O(m + |M|)
// |M| is the number of pairs (i, j) such that s1[i] = s2[j]
// return: the length of the longest common subsequence between s1 and s2
int sparse_LCS(const std::string &s1, const std::string &s2)
{
	int m = s1.size();
	int n = s2.size();
	return ((m + n - sparse_edit_distance(s1, s2)) / 2);
}

int main()
{
	std::string s1 = "ABCD";
	std::string s2 = "EACBD";
	std::cout << sparse_LCS(s1, s2) << std::endl; // 
	return 0;
}