#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>

int overlap_alignment(std::string s1, std::string s2, int (*s)(char, char), int gap_penalty)
{
	assert(gap_penalty >= 0);
	int m = s1.size();
	int n = s2.size();
	std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
	for (int j = 1; j <= n; j++)
		dp[0][j] = -j * gap_penalty;
	for (int i = 1; i <= m; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			dp[i][j] = dp[i - 1][j - 1] + s(s1[i - 1], s2[j - 1]);
			dp[i][j] = std::max(dp[i][j], dp[i - 1][j] - gap_penalty);
			dp[i][j] = std::max(dp[i][j], dp[i][j - 1] - gap_penalty);
		}
	}
	int max_score = 0;
	for (int j = 0; j <= n; j++)
		max_score = std::max(max_score, dp[m][j]);
	return max_score;
}

// int main()
// {
// 	std::string s1 = "AGTACGCA";
// 	std::string s2 = "ACTCAGC";
// 	int gap_penalty = 1;
// 	int score = overlap_alignment(s1, s2, [](char a, char b) { return a == b ? 1 : -1; }, gap_penalty);
// 	std::cout << "Overlap alignment score: " << score << std::endl;
// 	return 0;
// }
