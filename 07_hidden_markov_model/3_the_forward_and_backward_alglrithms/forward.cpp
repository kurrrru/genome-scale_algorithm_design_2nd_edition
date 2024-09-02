#include <bits/stdc++.h>

double forward_dp(const std::vector<int> &seq, const std::vector<std::vector<double>> &trans, const std::vector<std::vector<double>> &emit, std::vector<std::vector<double>> &f_dp)
{
	int N = seq.size();
	int H = trans.size();
	f_dp.assign(N + 2, std::vector<double>(H, 0));
	f_dp[0][0] = 1;
	for (int i = 0; i < N; i++)
	{
		for (int h = 1; h < H - 1; h++)
		{
			double prob = 0;
			for (int h_prev = 0; h_prev < H - 1; h_prev++)
			{
				prob += f_dp[i][h_prev] * trans[h_prev][h];
			}
			f_dp[i + 1][h] = prob * emit[h][seq[i]];
		}
	}
	for (int h = 1; h < H - 1; h++)
	{
		f_dp[N + 1][H - 1] += f_dp[N][h] * trans[h][H - 1];
	}
	return f_dp[N + 1][H - 1];
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
	std::vector<std::vector<double>> f_dp;
	double prob = forward_dp(seq, trans, emit, f_dp);
	std::cout << "Probability: " << prob << std::endl;
	std::cout << "DP Table:" << std::endl;
	std::cout << std::fixed;
	for (int i = 0; i < f_dp.size(); i++)
	{
		for (int j = 0; j < f_dp[i].size(); j++)
		{
			std::cout << f_dp[i][j] << " ";
		}
		std::cout << std::endl;
	}
	return 0;
}
