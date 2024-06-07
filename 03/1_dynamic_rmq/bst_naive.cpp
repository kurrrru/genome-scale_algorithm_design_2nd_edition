#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

static int bit_width(int x)
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
	bst_naive(Value _inf) : root(nullptr), inf(_inf), size(0), capacity(0) {}

	void build(const vector<Key> &keys)
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
	Value	rmq(const Key l, const Key r)
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

	void build_rec(node **cur, const vector<Key> &keys, int l, int r)
	{
		if (l >= size || l == r) return ;
		*cur = new node(keys.at(min(r - 1, size - 1)), inf);
		if (r == l + 1) return ;
		int m = (l + r) / 2;
		build_rec(&((*cur)->left), keys, l, m);
		build_rec(&((*cur)->right), keys, m, r);
	}

	Value	rmq_rec(node *cur, const Key l, const Key r)
	{
		if (cur == nullptr || cur->key < l) return (inf);
		if (cur->left && cur->left->key >= l && cur->right && cur->right->key <= r)
			return (min(cur->right->value, rmq_rec(cur->left, l, r)));
		if (cur->left && (cur->left->key >= r || cur->right == nullptr))
			return (rmq_rec(cur->left, l, r));
		if (cur->left && cur->left->key < l)
			return (rmq_rec(cur->right, l, r));
		if (cur->left && cur->right)
			return (min(rmq_rec(cur->left, l, r), rmq_rec(cur->right, l, r)));
		return (cur->value);
	}

	bool	update_rec(node *cur, Key idx, Value val)
	{
		if (cur == nullptr || cur->key < idx) return (false);
		bool res = false;
		if (cur->key > idx)
			res = update_rec(cur->left, idx, val) || update_rec(cur->right, idx, val); // 違うかも
		else if (cur->key == idx && cur->right != nullptr)
			res = update_rec(cur->right, idx, val);
		else
		{
			cur->value = val;
			return (true);
		}
		if (res && cur->left && cur->right)
			cur->value = min(cur->left->value, cur->right->value);
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

// int main(void)
// {
// 	vector<int> keys = {1, 11, 12, 41, 44, 49, 57, 58, 99};
// 	bst_naive<int, int> bst(1 << 30);
// 	bst.build(keys);

// 	cout << "bst.rmq(1,100)" << endl;
// 	cout << "[expect] 1073741824" << endl;
// 	cout << "[result] " << bst.rmq(1, 100) << endl;

// 	bst.update(44, 1000);
// 	bst.update(49, 300);
// 	bst.update(11, 200);
// 	bst.update(50, 10);

// 	cout << "bst.rmq(1,100)" << endl;
// 	cout << "[expect] 200" << endl;
// 	cout << "[result] " << bst.rmq(1, 100) << endl;

// 	cout << "bst.rmq(40,50)" << endl;
// 	cout << "[expect] 300" << endl;
// 	cout << "[result] " << bst.rmq(40, 50) << endl;

// 	bst.update(99, 20);

// 	cout << "bst.rmq(1,100)" << endl;
// 	cout << "[expect] 20" << endl;
// 	cout << "[result] " << bst.rmq(1, 100) << endl;

// 	bst.update(1, 2000);
// 	cout << "bst.rmq(-3,1)" << endl;
// 	cout << "[expect] 2000" << endl;
// 	cout << "[result] " << bst.rmq(-3, 1) << endl;

// 	bst.update(99, 30);
// 	cout << "bst.rmq(1,100)" << endl;
// 	cout << "[expect] 30" << endl;
// 	cout << "[result] " << bst.rmq(1, 100) << endl;

// 	return (0);
// }
