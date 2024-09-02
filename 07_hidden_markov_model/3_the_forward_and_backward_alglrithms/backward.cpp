#include <bits/stdc++.h>

double backward_dp(const std::vector<int> &seq, const std::vector<std::vector<double>> &trans, const std::vector<std::vector<double>> &emit, std::vector<std::vector<double>> &b_dp)
{
	int N = seq.size();
	int H = trans.size();
	b_dp.assign(N + 2, std::vector<double>(H, 0));
	b_dp[N + 1][H - 1] = 1;
	for (int i = N - 1; i >= 0; i--)
	{
		for (int h = 1; h < H - 1; h++)
		{
			double prob = 0;
			for (int h_next = 1; h_next < H; h_next++)
			{
				prob += b_dp[i + 2][h_next] * trans[h][h_next];
			}
			b_dp[i + 1][h] = prob * emit[h][seq[i]];
		}
	}
	for (int h = 1; h < H - 1; h++)
	{
		b_dp[0][0] += b_dp[1][h] * trans[0][h];
	}
	return b_dp[0][0];
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
	std::vector<std::vector<double>> b_dp;
	double prob = backward_dp(seq, trans, emit, b_dp);
	std::cout << "Probability: " << prob << std::endl;
	std::cout << "DP Table:" << std::endl;
	std::cout << std::fixed;
	for (int i = 0; i < b_dp.size(); i++)
	{
		for (int j = 0; j < b_dp[i].size(); j++)
		{
			std::cout << b_dp[i][j] << " ";
		}
		std::cout << std::endl;
	}
	return 0;
}
