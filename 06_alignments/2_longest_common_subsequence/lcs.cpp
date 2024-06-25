#include <bits/stdc++.h>

std::string LCS(const std::string &s1, const std::string &s2)
{
	int n = s1.size();
	int m = s2.size();
	std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++)
			dp[i][j] = s1[i - 1] == s2[j - 1] ? dp[i - 1][j - 1] + 1 : std::max(dp[i - 1][j], dp[i][j - 1]);
	std::string res = "";
	int i = n, j = m;
	while (i > 0 && j > 0)
	{
		if (s1[i - 1] == s2[j - 1])
		{
			res = s1[i - 1] + res;
			i--;
			j--;
		}
		else if (dp[i - 1][j] > dp[i][j - 1])
			i--;
		else
			j--;
	}
	return res;
}

int main()
{
	std::string s1 = "AGGTAB";
	std::string s2 = "GXTXAYB";
	std::cout << LCS(s1, s2) << std::endl; // GTAB
	return 0;
}
