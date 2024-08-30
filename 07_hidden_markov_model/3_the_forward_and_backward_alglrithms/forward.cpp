#include <bits/stdc++.h>

double forward_dp(const std::vector<int> &seq, const std::vector<std::vector<double>> &trans, const std::vector<std::vector<double>> &emit, std::vector<std::vector<double>> &dp)
{
	int N = seq.size();
	int H = trans.size();
	dp.resize(N + 2, std::vector<double>(H, 0));
	dp[0][0] = 1;
	for (int i = 0; i < N; i++)
	{
		for (int h = 1; h < H - 1; h++)
		{
			double prob = 0;
			for (int h_prev = 0; h_prev < H - 1; h_prev++)
			{
				prob += dp[i][h_prev] * trans[h_prev][h];
			}
			dp[i + 1][h] = prob * emit[h][seq[i]];
		}
	}
	for (int h = 1; h < H - 1; h++)
	{
		dp[N + 1][H] += dp[N][h] * trans[h][H - 1];
	}
	return dp[N + 1][H];
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
	std::vector<std::vector<double>> dp;
	double prob = forward_dp(seq, trans, emit, dp);
	std::cout << "Probability: " << prob << std::endl;
	std::cout << "DP Table:" << std::endl;
	for (int i = 0; i < dp.size(); i++)
	{
		for (int j = 0; j < dp[i].size(); j++)
		{
			std::cout << dp[i][j] << " ";
		}
		std::cout << std::endl;
	}
	return 0;
}
