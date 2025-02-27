// 计算两个开头结尾不同  长度相同 的字符串的最长公共子序列长度  (只判断长度，其余的不管)
#include <cstdio>
#include<string>
#include<vector>
#include<fstream>
using namespace std;


string removeSpace(string s)
{
	string  res="";
	for(char c:s)
	{
		if(c !=' ')
		res+=c;
	}
	return res;
}
int compute_xy_lcs_length(string s1,string s2) {
	s1 = removeSpace(s1);
	s2 = removeSpace(s2);
    if(s1.length() != s2.length() || s1[0] == s2[0] || s1[s1.length()-1] == s2[s1.length()-1])
    {
       // printf("输入序列不符合要求(长度不相等 或存在收尾相同");
        return 0;
    }
    int  n=s1.length();
    vector<vector<int>> dp(n+1,vector<int>(n+1,0));

	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			dp[i][j] = dp[i - 1][j - 1] ;
			if (s1[i] == s2[j] )
			{
				dp[i][j] += 1;
			}
			else
			{
				dp[i][j] = max(dp[i][j - 1], dp[i - 1][j]);
			}
		}
	}
	vector<int>  ans(n +1);
	for (int i = 0; i <= n; i++)
	{
		ans[i] = dp[i][i];
	}
  //  printf("序列长度： %d,  |lcs| = %d ,   d_L = %d \n",n,ans[n], n-ans[n]);
	return n-ans[n];
};     