#include <bits/stdc++.h>

double viterbi_square_partition_dp(const std::vector<int> &seq, const std::vector<std::vector<double>> &trans, const std::vector<std::vector<double>> &emit, std::vector<std::vector<double>> &v_dp, int block_size = -1)
{
	const int N = seq.size();
	const int H = trans.size();
	if (block_size == -1)
	{
		block_size = sqrt(N);
	}
	v_dp.assign((N - 1) / block_size + 3, std::vector<double>(H));
	v_dp[0][0] = 1;
	int block = 0;
	for (int i = 0; i < N; i++)
	{
		std::vector<double> tmp(H);
		for (int h = 1; h < H - 1; h++)
		{
			double max_prob = 0;
			for (int h_prev = 0; h_prev < H - 1; h_prev++)
			{
				max_prob = std::max(max_prob, v_dp[block][h_prev] * trans[h_prev][h]);
			}
			tmp[h] = max_prob * emit[h][seq[i]];
		}
		if (i % block_size == 0)
		{
			block++;
		}
		std::swap(v_dp[block], tmp);
	}
	std::vector<double> last_row(H);
	for (int h = 1; h < H - 1; h++)
	{
		last_row[H - 1] = std::max(last_row[H - 1], v_dp[block][h] * trans[h][H - 1]);
	}
	std::swap(v_dp[++block], last_row);
	return v_dp[block][H - 1];
}

std::vector<int> viterbi_square_partition_traceback(const std::vector<int> &seq, const std::vector<std::vector<double>> &trans, const std::vector<std::vector<double>> &emit, const std::vector<std::vector<double>> &v_dp, int block_size = -1)
{
	const int N = seq.size();
	const int H = trans.size();
	const int num_blocks = (N - 1) / block_size + 3;
	if (block_size == -1)
	{
		block_size = sqrt(N);
	}
	std::vector<int> path(N + 2, 0);
	path[N + 1] = H - 1;
	for (int block = num_blocks - 3; block >= 0; block--)
	{
		int start = block * block_size;
		int end = std::min((block + 1) * block_size, N);
		std::vector<std::vector<double>> tmp(end - start + 1, std::vector<double>(H));
		tmp[0] = v_dp[block];
		for (int i = start; i < end; i++)
		{
			for (int h = 1; h < H - 1; h++)
			{
				double max_prob = 0;
				for (int h_prev = 0; h_prev < H - 1; h_prev++)
				{
					max_prob = std::max(max_prob, tmp[i - start][h_prev] * trans[h_prev][h]);
				}
				tmp[i - start + 1][h] = max_prob * emit[h][seq[i]];
			}
		}
		for (int h = 1; h < H - 1; h++)
		{
			if (path[end + 1] == H - 1 && v_dp[block + 2][path[end + 1]] == tmp[end - start][h] * trans[h][path[end + 1]])
			{
				path[end] = h;
				break;
			}
			else if (path[end + 1] != H - 1 && v_dp[block + 2][path[end + 1]] == tmp[end - start][h] * trans[h][path[end + 1]] * emit[path[end + 1]][seq[end]])
			{
				path[end] = h;
				break;
			}
		}
		for (int i = end - 1; i >= start; i--)
		{
			for (int h = 1; h < H - 1; h++)
			{
				if (tmp[i - start + 1][path[i + 1]] == tmp[i - start][h] * trans[h][path[i + 1]] * emit[path[i + 1]][seq[i]])
				{
					path[i] = h;
					break;
				}
			}
		}
	}
	return path;
}

int main()
{
	std::vector<int> seq = {0, 1, 2, 1, 0};
	std::vector<std::vector<double>> trans(
		{
			{0.0, 0.3, 0.7, 0.0},
			{0.0, 0.5, 0.4, 0.1},
			{0.0, 0.6, 0.3, 0.1},
			{0.0, 0.0, 0.0, 0.0},
		});
	std::vector<std::vector<double>> emit(
		{
			{0.0, 0.0, 0.0},
			{0.1, 0.4, 0.5},
			{0.6, 0.3, 0.1},
			{0.0, 0.0, 0.0},
		});
	std::vector<std::vector<double>> v_dp;
	int block_size = sqrt(seq.size());
	double prob = viterbi_square_partition_dp(seq, trans, emit, v_dp, block_size);
	std::cout << "Probability: " << prob << std::endl;
	std::cout << "DP Table:" << std::endl;
	std::cout << std::fixed;
	for (int i = 0; i < v_dp.size(); i++)
	{
		for (int j = 0; j < v_dp[i].size(); j++)
		{
			std::cout << v_dp[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::vector<int> path = viterbi_square_partition_traceback(seq, trans, emit, v_dp, block_size);
	std::cout << "Path: ";
	for (int i = 0; i < path.size(); i++)
	{
		std::cout << path[i] << " ";
	}
	std::cout << std::endl;
	return 0;
}
