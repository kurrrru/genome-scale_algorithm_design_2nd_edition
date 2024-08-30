#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

// #define DISPLAY_DP

#ifdef DISPLAY_DP
void display_dp(const std::string &s1, const std::string &s2,
		const std::vector<std::vector<int> > &dp, int m, int n, int x)
{
	int w = 2;

	std::cout << std::setw(2 * w + 1) << " " << " ";
	for (int j = 0; j <= n; j++)
		std::cout << std::setw(w) << s1[j] << " ";
	std::cout << std::endl;
	for (int i = 0; i <= m; i++)
	{
		if (i == 0)
			std::cout << std::setw(w) << " " << " ";
		else
			std::cout << std::setw(w) << s2[i - 1] << " ";
		for (int k = 0; k < i - x; k++)
			std::cout << std::setw(w) << "0" << " ";
		std::cout << "\x1b[31m";
		for (int j = 0; j < (int)dp[i].size(); j++)
		{
			std::cout << std::setw(w) << dp[i][j] << " ";
		}
		std::cout << "\x1b[0m";
		for (int k = 0; k <= n - (int)dp[i].size() - std::max(i - x, 0); k++)
			std::cout << std::setw(w) << "0" << " ";
		std::cout << std::endl;
	}
}
#endif

int restricted_edit_distance(const std::string &s1, const std::string &s2, int t)
{
	int n = s1.size();
	int m = s2.size();
	if (n < m)
		return restricted_edit_distance(s2, s1, t);
	if (n - m > t)
		return -1;
	if (m == 0)
		return n;
	int x = (t - (n - m) + 1) / 2;
	int mat = 0, mis = 1, ins = 1, del = 1;
	std::vector<std::vector<int> > dp(m + 1);
	for (int i = 0; i <= m; i++)
	{
		dp[i].resize(n - m + 2 * x + 1 - std::max(x - i, 0) - std::max(x + i - m, 0), 0);
		if (i == 0)
			for (int j = 1; j <= n - m + x; j++)
				dp[i][j] = dp[i][j - 1] + ins;
		else if (i <= x)
		{
			dp[i][0] = dp[i - 1][0] + del;
			for (int j = 1; j <= n - m + x + i; j++)
				dp[i][j] = std::min(dp[i - 1][j - 1] + (s2[i - 1] == s1[j - 1] ? mat : mis), std::min(dp[i][j - 1] + ins, dp[i - 1][j] + del));
		}
		else
		{
			dp[i][0] = std::min(dp[i - 1][0] + (s2[i - 1] == s1[i - x - 1] ? mat : mis), dp[i - 1][1] + del);
			for (int j = 1; j <= n - m + 2 * x; j++)
			{
				if (i - x + j - 1 >= n)
					break ;
				if (j < n - m + 2 * x)
					dp[i][j] = std::min(dp[i - 1][j] + (s2[i - 1] == s1[i - x + j - 1] ? mat : mis), std::min(dp[i][j - 1] + ins, dp[i - 1][j + 1] + del));
				else
					dp[i][j] = std::min(dp[i - 1][j] + (s2[i - 1] == s1[i - x + j - 1] ? mat : mis), dp[i][j - 1] + ins);
			}
		}
	}
#ifdef DISPLAY_DP
	display_dp(s1, s2, dp, m, n, x);
#endif
	if (dp[m][n - m + x] > t)
		return -1;
	return dp[m][n - m + x];
}

int shortest_detour(const std::string &s1, const std::string &s2)
{
	int t = 1;
	int diff = -1;
	while (diff == -1)
	{
		diff = restricted_edit_distance(s1, s2, t);
		t *= 2;
	}
	return diff;
}

int main()
{
	std::string s1 = "tervetuloa";
	std::string s2 = "teretulemast";
	std::cout << shortest_detour(s1, s2) << std::endl;
	return 0;
}
