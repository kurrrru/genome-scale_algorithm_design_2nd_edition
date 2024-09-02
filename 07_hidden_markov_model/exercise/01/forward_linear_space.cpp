#include <bits/stdc++.h>

double forward_dp_linear_space(const std::vector<int> &seq, const std::vector<std::vector<double>> &trans, const std::vector<std::vector<double>> &emit, std::vector<double> &f_dp)
{
	int N = seq.size();
	int H = trans.size();
	f_dp.assign(H, 0);
	f_dp[0] = 1;
	for (int i = 0; i < N; i++)
	{
		std::vector<double> tmp(H);
		for (int h = 1; h < H - 1; h++)
		{
			double prob = 0;
			for (int h_prev = 0; h_prev < H - 1; h_prev++)
			{
				prob += f_dp[h_prev] * trans[h_prev][h];
			}
			tmp[h] = prob * emit[h][seq[i]];
		}
		std::swap(f_dp, tmp);
	}
	for (int h = 1; h < H - 1; h++)
	{
		f_dp[H - 1] += f_dp[h] * trans[h][H - 1];
	}
	return f_dp[H - 1];
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
	std::vector<double> f_dp;
	double prob = forward_dp_linear_space(seq, trans, emit, f_dp);
	std::cout << "Probability: " << prob << std::endl;
	return 0;
}
