#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int cost(char a, char b)
{
	int match = 0, mismatch = 1, gap = 1;
	if (a == b)
		return match;
	if (a == '-' ^ b == '-')
		return gap;
	return mismatch;
}

int edit_distance(string a, string b)
{
	int n = a.size(), m = b.size();
	vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
	for (int i = 1; i <= n; i++)
		dp[i][0] = i;
	for (int j = 1; j <= m; j++)
		dp[0][j] = j;
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++)
		{
			dp[i][j] = dp[i - 1][j - 1] + cost(a[i - 1], b[j - 1]);
			dp[i][j] = min(dp[i][j], dp[i - 1][j] + cost(a[i - 1], '-'));
			dp[i][j] = min(dp[i][j], dp[i][j - 1] + cost('-', b[j - 1]));
		}
	return dp[n][m];
}

int main()
{
	string a = "tervetuloa";
	string b = "teretulemast";
	cout << edit_distance(a, b) << endl;
	return 0;
}
