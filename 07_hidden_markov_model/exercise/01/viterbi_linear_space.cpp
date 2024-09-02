#include <bits/stdc++.h>

double viterbi_dp_linear_space(const std::vector<int> &seq, const std::vector<std::vector<double>> &trans, const std::vector<std::vector<double>> &emit, std::vector<double> &v_dp)
{
	int N = seq.size();
	int H = trans.size();
	v_dp.assign(H, 0);
	v_dp[0] = 1;
	for (int i = 0; i < N; i++)
	{
		std::vector<double> tmp(H);
		for (int h = 1; h < H - 1; h++)
		{
			double max_prob = 0;
			for (int h_prev = 0; h_prev < H - 1; h_prev++)
			{
				max_prob = std::max(max_prob, v_dp[h_prev] * trans[h_prev][h]);
			}
			tmp[h] = max_prob * emit[h][seq[i]];
		}
		std::swap(v_dp, tmp);
	}
	for (int h = 1; h < H - 1; h++)
	{
		v_dp[H - 1] = std::max(v_dp[H - 1], v_dp[h] * trans[h][H - 1]);
	}
	return v_dp[H - 1];
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
	std::vector<double> v_dp;
	double prob = viterbi_dp_linear_space(seq, trans, emit, v_dp);
	std::cout << "Probability: " << prob << std::endl;
	return 0;
}
