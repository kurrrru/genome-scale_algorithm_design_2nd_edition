#include <bits/stdc++.h>

double backward_dp(const std::vector<int> &seq, const std::vector<std::vector<double>> &trans, const std::vector<std::vector<double>> &emit, std::vector<double> &b_dp)
{
	int N = seq.size();
	int H = trans.size();
	b_dp.assign(H, 0);
	b_dp[H - 1] = 1;
	for (int i = N - 1; i >= 0; i--)
	{
		std::vector<double> tmp(H);
		for (int h = 1; h < H - 1; h++)
		{
			double prob = 0;
			for (int h_next = 1; h_next < H; h_next++)
			{
				prob += b_dp[h_next] * trans[h][h_next];
			}
			tmp[h] = prob * emit[h][seq[i]];
		}
		std::swap(b_dp, tmp);
	}
	for (int h = 1; h < H - 1; h++)
	{
		b_dp[0] += b_dp[h] * trans[0][h];
	}
	return b_dp[0];
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
	std::vector<double> b_dp;
	double prob = backward_dp(seq, trans, emit, b_dp);
	std::cout << "Probability: " << prob << std::endl;
	return 0;
}
