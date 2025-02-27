#include<iostream>
#include "lcs.h"
using namespace std;

ofstream fileExpSucc,fileExpFail,fileUnexpSucc,fileUnexpFail;

bool OpenAllFiles(string dir)
{
     // 打开文件，模式为追加
    fileExpSucc.open(dir+"/ExpSucc.txt", ios::out);
    fileExpFail.open(dir+"/ExpFail.txt", ios::out);
    fileUnexpSucc.open(dir+"/UnexpSucc.txt", ios::out);
    fileUnexpFail.open(dir+"/UnexpFail.txt", ios::out);

 	// 检查文件是否成功打开
 	if (!fileExpSucc.is_open() || !fileExpFail.is_open() || !fileUnexpSucc.is_open() || !fileUnexpFail.is_open()) 
	{
		cout <<"DIR:"<< dir<< "  , cannot open file" << endl;
		return false;
	}
     return true;
}

// 函数：生成互补序列
string complement(const string& segment) {
    std::string result;
    for (char c : segment) {
        result += (c == '0') ? '1' : '0';
    }
    return result;
}

// 函数：处理01序列
string FindcoupleOfX(const string& s) {
    string seg="";
    std::vector<string> segments;

    // 分割字符串
    for(char c:s)
	{
		if(c==' ')
		{
			segments.push_back(seg);
			seg="";
		}
		else
		{
			seg+=c;
		}
	}
	std::string result;
    for (size_t i = 0; i < segments.size(); ++i) {
        if (i % 2 == 0) {
            // 奇数位置的小段（从0开始计数，所以用 i % 2 == 0）
            result += complement(segments[i]) + " ";
        } else {
            // 偶数位置的小段
            result += segments[i] + " ";
        }
	}
    return result;
}


/* 递归生成序列 
输入:  目标距离    当前要生成的段标号  当前段开头字符  目前的序列内容  当前序列是否满足结构条件
输出： 输出判断结果到四个文件
1: 符合预期-且距离要求满足的
2：符合预期但距离要求不满足的
3：预期满足 实际上不仅满足
4：预期不满足 实际上满足

对于满足距离要求的 需要去求所有的公共子序列 
文件格式
1. s  [距离]  "YES/NO是否满足达到最大交集" (最长公共子序列数目)-xxxx -xxxx -xxxxx 公共子序列
2. s [距离]
3.  
*/
/* 处理第一种情况 符合预期 满足距离要求*/

void HandleCaseExpSucc(string s,int d,ofstream &myfile)
{
	myfile<< s+"  \"YES\" ["+to_string(d)+"]\n" ;
	//cout<< s+"  \"YES\" ["+to_string(d)+"]\n" ;
}

/* 处理第一种情况 符合预期 不满足距离要求*/
void HandleCaseExpFail(string s,int d,ofstream &myfile)
{
	myfile<< s+"  \"No\" ["+to_string(d)+"]\n" ;
	//cout<< s+"  \"NO\" ["+to_string(d)+"]\n" ;
}
/* 第三种情况，意外之喜 */
void HandleCaseUnExpSucc(string s,int d,ofstream &myfile)
{
	myfile<< s+"  \"No\" ["+to_string(d)+"]\n" ;
	//cout<< s+"  \"YES\" ["+to_string(d)+"]\n" ;

}

/* 第四种情况，不及预期 期望符合实际不符合*/
void HandleCaseUnExpFail(string s,int d,ofstream &myfile)
{
	myfile<< s+"  \"YES\" ["+to_string(d)+"]\n" ;
	//cout<< s+"  \"NO\" []"+to_string(d)+"]\n" ;
}


/* 已知当前序列，求下一段序列 */
std::string gen_next_string(string &s,char c, int k) {
    string result = s;
    char current = c; // 当前字符，初始化为 c
    for (int i = 0; i < k; ++i) {
        result += current; // 将当前字符添加到结果中
        current = (current == '0') ? '1' : '0'; // 交替字符
    }
    return result+' ';
} 

void ClassfiyAndCompeteSubsequence(int t,int idx,string nows,char beginc,bool leftreverse,bool checkcons)
{
	/* 已经把所有的段都赋值好了 根据序列计算出互补序列*/
	if(idx == 2*t)
	{
		string x=nows,y = FindcoupleOfX(nows);
		int d = compute_xy_lcs_length(x,y);   /* 满足结构且距离等于t   在预期内的成功*/
		if ( checkcons && d >= t)
		{
			HandleCaseExpSucc(x,d,fileExpSucc);
		}
		else if(checkcons)   /* 满足描述结构，但距离小于t  未达预期+失败 */
		{
			HandleCaseUnExpFail(x,d,fileUnexpFail);
		}
		else if (d >=  t)  /* 虽然没有满足结构 但仍人有足够的距离*/
		{
			HandleCaseUnExpSucc(x,d,fileUnexpSucc);
		}
		else   /* 没有满足结构，且距离小于t  在预期内的失败*/
		{
			HandleCaseExpFail(x,d,fileExpFail);
		}
		return;
	}


	/*   开始进行递归的逻辑  依次生成长度为2-idx的  多加一个长度 看看结果 */
	for(int i=2;i<=(min(idx,2*t-idx))+2;i++)
	{
		/* 
		   目前是否仍然满足结构怎么求? 
		   原来不满足结构，现在一定不满足
		   原来满足+ 现在完全翻转  一定满足
		   原来满足+现在不完全翻转但长度大于等于要求 
		
		*/
		string ns;   //新的字符串
		char barbaginc = (beginc=='0'?'1':'0');   //本段开头字符相反的字符
		bool rightreserved;     //右边是否翻转
		bool newCheckCons;        //增加了该段之后判断是不是仍然满足要求的条件
		
	    /* 下一段第一个符号和本段第一个一样： 根据本段长度，可以判断右边是否反转 再结合左边翻转情况就知道是不是满足结构要求*/
		ns = gen_next_string(nows,beginc,i);
		rightreserved = i%2;
		if(checkcons)
			newCheckCons = (i > idx) || (leftreverse == rightreserved);
		else
			newCheckCons = false; 
		ClassfiyAndCompeteSubsequence(t,idx+1,ns,beginc,rightreserved,newCheckCons) ;     //ps:  && i<=(min(idx,2*t-idx))  修改为  && (i<=(min(idx,2*t-idx)))    &&比<=的优先级高
	

		/* 下一段第一个符号和本段相反 ,此时如果本段长度为偶数则为反转  101 01xxx    1010 01xxx*/
		ns = gen_next_string(nows,barbaginc,i);
		rightreserved = (i-1)%2;
		if(checkcons)
			newCheckCons = (i > idx) || (leftreverse == rightreserved);
		else
			newCheckCons = false; 
		ClassfiyAndCompeteSubsequence(t,idx+1,ns,barbaginc,rightreserved,newCheckCons) ;

	}
}



