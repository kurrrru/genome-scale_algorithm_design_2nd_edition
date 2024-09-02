#include <bits/stdc++.h>

double forward_dp(const std::vector<int> &seq, const std::vector<std::vector<double>> &trans, const std::vector<std::vector<double>> &emit, std::vector<std::vector<double>> &f_dp)
{
	const int N = seq.size();
	const int H = trans.size();
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

double backward_dp(const std::vector<int> &seq, const std::vector<std::vector<double>> &trans, const std::vector<std::vector<double>> &emit, std::vector<std::vector<double>> &b_dp)
{
	const int N = seq.size();
	const int H = trans.size();
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

void estimate_ec(const std::vector<int> &seq, const std::vector<std::vector<double>> &trans, const std::vector<std::vector<double>> &emit, const std::vector<std::vector<double>> &f_dp, const std::vector<std::vector<double>> &b_dp, std::vector<std::vector<double>> &ec)
{
	const int N = seq.size();
	const int H = trans.size();
	const int K = emit[0].size();
	const double prob_s = f_dp[N + 1][H - 1];
	ec.assign(H, std::vector<double>(K, 0));
	for (int i = 0; i < N; i++)
	{
		for (int h = 1; h < H - 1; h++)
		{
			for (int h_prev = 0; h_prev < H - 1; h_prev++)
			{
				ec[h][seq[i]] += f_dp[i][h_prev] * trans[h_prev][h] * b_dp[i + 1][h] / prob_s;
			}
		}
	}
}

void estimate_tc(const std::vector<int> &seq, const std::vector<std::vector<double>> &trans, const std::vector<std::vector<double>> &emit, const std::vector<std::vector<double>> &f_dp, const std::vector<std::vector<double>> &b_dp, std::vector<std::vector<double>> &tc)
{
	const int N = seq.size();
	const int H = trans.size();
	const double prob_s = f_dp[N + 1][H - 1];
	tc.assign(H, std::vector<double>(H, 0));
	for (int i = 0; i < N + 1; i++)
	{
		for (int h_prev = 0; h_prev < H - 1; h_prev++)
		{
			for (int h = 1; h < H; h++)
			{
				tc[h_prev][h] += f_dp[i][h_prev] * trans[h_prev][h] * b_dp[i + 1][h] / prob_s;
			}
		}
	}
}

void normalize(std::vector<std::vector<double>> &v)
{
	for (int i = 0; i < v.size(); i++)
	{
		double sum = std::accumulate(v[i].begin(), v[i].end(), 0.0);
		if (sum == 0)
		{
			continue;
		}
		for (int j = 0; j < v[i].size(); j++)
		{
			v[i][j] /= sum;
		}
	}
}

double baum_welch(const std::vector<int> &seq, std::vector<std::vector<double>> &trans, std::vector<std::vector<double>> &emit, int max_iter, double eps = 1e-6)
{
	const int N = seq.size();
	const int H = trans.size();
	std::vector<std::vector<double>> f_dp, b_dp;
	double prob = 0;
	for (int iter = 0; iter < max_iter; iter++)
	{
		forward_dp(seq, trans, emit, f_dp);
		if (std::abs(prob - f_dp[N + 1][H - 1]) < eps)
		{
			break;
		}
		prob = f_dp[N + 1][H - 1];
		backward_dp(seq, trans, emit, b_dp);
		std::vector<std::vector<double>> ec, tc;
		estimate_ec(seq, trans, emit, f_dp, b_dp, ec);
		estimate_tc(seq, trans, emit, f_dp, b_dp, tc);
		normalize(ec);
		normalize(tc);
		std::swap(emit, ec);
		std::swap(trans, tc);
	}
	return prob;
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
	baum_welch(seq, trans, emit, 100, 1e-10);
	return 0;
}
