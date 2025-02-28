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
// int compute_xy_lcs_length(string s1,string s2, vector<vector<int>> &dp) {
// 	s1 = removeSpace(s1);
// 	s2 = removeSpace(s2);
//     if(s1.length() != s2.length() || s1[0] == s2[0] || s1[s1.length()-1] == s2[s1.length()-1])
//     {
//        // printf("输入序列不符合要求(长度不相等 或存在收尾相同");
//         return 0;
//     }
//     int  n=s1.length();
// 	for (int i = 1; i <= n; i++)
// 	{
// 		for (int j = 1; j <= n; j++)
// 		{
// 			dp[i][j] = dp[i - 1][j - 1] ;
// 			if (s1[i] == s2[j] )
// 			{
// 				dp[i][j] += 1;
// 			}
// 			else
// 			{
// 				dp[i][j] = max(dp[i][j - 1], dp[i - 1][j]);
// 			}
// 		}
// 	}
// 	return dp[n][n];
// };     





/*求出所有的最长公共子序列*/
// 动态规划求解并输出所有LCS
#include <iostream>
#include <string>
#include <vector>
#include <set>
using namespace std;

int max(int a, int b)
{
	return (a>b)? a:b;
}
 
/** 
 * 字符串逆序
 */
string Reverse(string str)
{
	int low = 0;
	int high = str.length() - 1;
	while (low < high)
	{
		char temp = str[low];
		str[low] = str[high];
		str[high] = temp;
		++low;
		--high;
	}
	return str;
}
 


int lcs(int m, int n,string X,string Y,vector<vector<int>> &table)
{ 
	for(int i=0; i<m+1; ++i)
	{
		for(int j=0; j<n+1; ++j)
		{
			// 第一行和第一列置0
			if (i == 0 || j == 0)
				table[i][j] = 0;
 
			else if(X[i-1] == Y[j-1])
				table[i][j] = table[i-1][j-1] + 1;
 
			else
				table[i][j] = max(table[i-1][j], table[i][j-1]);
		}
	}
 
	return table[m][n];
}
 
/** 
 * 求出所有的最长公共子序列，并放入set中 
 */
void traceBack(int i, int j, string &X,string &Y,string lcs_str,vector<vector<int>> &table,set<string> &setOfLCS)
{
	while (i>0 && j>0)
	{
		if (X[i-1] == Y[j-1])
		{
			lcs_str.push_back(X[i-1]);
			--i;
			--j;
		}
		else
		{
			if (table[i-1][j] > table[i][j-1])
				--i;
			else if (table[i-1][j] < table[i][j-1])
				--j;
			else   // 相等的情况
			{
				traceBack(i-1, j,X,Y, lcs_str,table,setOfLCS);
				traceBack(i, j-1,X,Y, lcs_str,table,setOfLCS);
				return;
			}
		}
	}
  
	setOfLCS.insert(Reverse(lcs_str));
}
 
 
set<string> getAllCommonSubStr(string s1,string s2,vector<vector<int>> &table,set<string> &setOfLCS)
{
	s1 = removeSpace(s1);
	s2 = removeSpace(s2);
	string str="";
	traceBack(s1.length(), s2.length(),s1,s2, str,table,setOfLCS);
	set<string>::iterator beg = setOfLCS.begin();
	return setOfLCS;
}




