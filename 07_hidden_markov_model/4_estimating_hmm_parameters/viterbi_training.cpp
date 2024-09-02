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

void estimate_ec(const std::vector<int> &seq, const std::vector<std::vector<double>> &trans, const std::vector<std::vector<double>> &emit, const std::vector<int> &path, std::vector<std::vector<double>> &ec)
{
	const int N = seq.size();
	const int H = trans.size();
	const int K = emit[0].size();
	ec.assign(H, std::vector<double>(K, 0));
	for (int i = 0; i < N; i++)
	{
		ec[path[i + 1]][seq[i]]++;
	}
}

void estimate_tc(const std::vector<int> &seq, const std::vector<std::vector<double>> &trans, const std::vector<std::vector<double>> &emit, const std::vector<int> &path, std::vector<std::vector<double>> &tc)
{
	const int N = seq.size();
	const int H = trans.size();
	tc.assign(H, std::vector<double>(H, 0));
	for (int i = 0; i < N + 1; i++)
	{
		tc[path[i]][path[i + 1]]++;
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

double viterbi_training(const std::vector<int> &seq, std::vector<std::vector<double>> &trans, std::vector<std::vector<double>> &emit, int max_iter, double eps = 1e-20)
{
	const int N = seq.size();
	const int H = trans.size();
	std::vector<std::vector<double>> v_dp;
	std::vector<int> path;
	double prob = 0;
	for (int iter = 0; iter < max_iter; iter++)
	{
		viterbi_dp(seq, trans, emit, v_dp);
		path = viterbi_traceback(seq, trans, emit, v_dp);
		if (std::abs(prob - v_dp[N + 1][H - 1]) < eps)
		{
			break;
		}
		prob = v_dp[N + 1][H - 1];
		std::vector<std::vector<double>> ec, tc;
		estimate_ec(seq, trans, emit, path, ec);
		estimate_tc(seq, trans, emit, path, tc);
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
	viterbi_training(seq, trans, emit, 100);
	return 0;
}
