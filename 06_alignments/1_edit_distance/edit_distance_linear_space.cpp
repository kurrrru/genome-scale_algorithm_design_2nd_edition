#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int cost(char a, char b)
{
	int match = 0, mismatch = 1, gap = 1;
	if (a == b)
		return match;
	if (a == '-' || b == '-')
		return gap;
	return mismatch;
}

int edit_distance(string a, string b)
{
	int n = a.size(), m = b.size();
	if (n < m)
		swap(a, b), swap(n, m);
	vector<int> dp(m + 1, 0);
	for (int j = 1; j <= m; j++)
		dp[j] = dp[j - 1] + cost('-', b[j - 1]);
	for (int i = 1; i <= n; i++)
	{
		int prev = dp[0];
		dp[0] = dp[0] + cost(a[i - 1], '-');
		for (int j = 1; j <= m; j++)
		{
			int tmp = dp[j];
			dp[j] = prev + cost(a[i - 1], b[j - 1]);
			dp[j] = min(dp[j], dp[j - 1] + cost('-', b[j - 1]));
			dp[j] = min(dp[j], tmp + cost(a[i - 1], '-'));
			prev = tmp;
		}
	}
	return dp[m];
}

int main()
{
	string a = "tervetuloa";
	string b = "teretulemast";
	cout << edit_distance(a, b) << endl;
	return 0;
}
