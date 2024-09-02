#include <bits/stdc++.h>

double viterbi_dp(const std::vector<int> &seq, const std::vector<std::vector<double>> &trans, const std::vector<std::vector<double>> &emit, std::vector<std::vector<double>> &v_dp)
{
	int N = seq.size();
	int H = trans.size();
	v_dp.assign(N + 2, std::vector<double>(H, 0));
	v_dp[0][0] = 1;
	for (int i = 0; i < N; i++)
	{
		for (int h = 1; h < H - 1; h++)
		{
			double max_prob = 0;
			for (int h_prev = 0; h_prev < H - 1; h_prev++)
			{
				max_prob = std::max(max_prob, v_dp[i][h_prev] * trans[h_prev][h]);
			}
			v_dp[i + 1][h] = max_prob * emit[h][seq[i]];
		}
	}
	for (int h = 1; h < H - 1; h++)
	{
		v_dp[N + 1][H - 1] = std::max(v_dp[N + 1][H - 1], v_dp[N][h] * trans[h][H - 1]);
	}
	return v_dp[N + 1][H - 1];
}

std::vector<int> viterbi_traceback(const std::vector<int> &seq, const std::vector<std::vector<double>> &trans, const std::vector<std::vector<double>> &emit, const std::vector<std::vector<double>> &v_dp)
{
	int N = seq.size();
	int H = trans.size();
	std::vector<int> path(N + 2, 0);
	path[N + 1] = H - 1;
	for (int h = 1; h < H - 1; h++)
	{
		if (v_dp[N + 1][H - 1] == v_dp[N][h] * trans[h][H - 1])
		{
			path[N] = h;
			break;
		}
	}
	for (int i = N - 1; i >= 0; i--)
	{
		for (int h = 1; h < H - 1; h++)
		{
			if (v_dp[i + 1][path[i + 1]] == v_dp[i][h] * trans[h][path[i + 1]] * emit[path[i + 1]][seq[i]])
			{
				path[i] = h;
				break;
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
	double prob = viterbi_dp(seq, trans, emit, v_dp);
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
	std::vector<int> path = viterbi_traceback(seq, trans, emit, v_dp);
	std::cout << "Path: ";
	for (int i = 0; i < path.size(); i++)
	{
		std::cout << path[i] << " ";
	}
	std::cout << std::endl;
	return 0;
}
