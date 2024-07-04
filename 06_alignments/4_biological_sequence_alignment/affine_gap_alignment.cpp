#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>

int global_alignment_under_affine_gap(std::string s1, std::string s2, int (*s)(char, char), int gap_open, int gap_extention)
{
	assert(gap_open >= 0);
	assert(gap_extention >= 0);
	int m = s1.size();
	int n = s2.size();
	int inf = 1 << 30;
	std::vector<std::vector<int>> sm(m + 1, std::vector<int>(n + 1, 0));
	std::vector<std::vector<int>> sg(m + 1, std::vector<int>(n + 1, 0));
	for (int i = 1; i <= m; i++)
		sm[i][0] = -inf;
	for (int j = 1; j <= n; j++)
		sm[0][j] = -inf;
	for (int i = 1; i <= m; i++)
		sg[i][0] = -gap_open - (i - 1) * gap_extention;
	for (int j = 1; j <= n; j++)
		sg[0][j] = -gap_open - (j - 1) * gap_extention;
	for (int i = 1; i <= m; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			sm[i][j] = sm[i - 1][j - 1] + s(s1[i - 1], s2[j - 1]);
			sm[i][j] = std::max(sm[i][j], sg[i - 1][j - 1] + s(s1[i - 1], s2[j - 1]));
			sg[i][j] = sm[i - 1][j] - gap_open;
			sg[i][j] = std::max(sg[i][j], sg[i - 1][j] - gap_extention);
			sg[i][j] = std::max(sg[i][j], sm[i][j - 1] - gap_open);
			sg[i][j] = std::max(sg[i][j], sg[i][j - 1] - gap_extention);
		}
	}
	return std::max(sm[m][n], sg[m][n]);
}

// int main()
// {
// 	std::string s1 = "ACTCCATC";
// 	std::string s2 = "ACGTCAATCAT";
// 	int gap_open = 1;
// 	int gap_extention = 1;
// 	int score = global_alignment_under_affine_gap(s1, s2, [](char a, char b) { return a == b ? 1 : -1; }, gap_open, gap_extention);
// 	std::cout << "Global alignment score under affine gap: " << score << std::endl;
// 	return 0;
// }
