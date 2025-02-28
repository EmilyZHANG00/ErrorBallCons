#include<iostream>
#include "lcs.h"
using namespace std;
// 文件流
ofstream fileSatifyAndSucc,fileSatifyAndFail,fileUnSatifyAndSucc,fileUnSatifyAndFail;
//是否输出具体的数值 (最好只有3 4输出  输出3的时候击败KB 输出4的时候已经几百MB了)
bool  NEED_OUTPUT_ALL_SUBSTR=false;
bool OpenAllFiles(string dir)
{
     // 打开文件，模式为追加
	 fileSatifyAndSucc.open(dir+"/SatifyAndSucc.txt", ios::out);
	 fileSatifyAndFail.open(dir+"/SatifyAndFail.txt", ios::out);
	 fileUnSatifyAndSucc.open(dir+"/UnSatifyAndSucc.txt", ios::out);
	 fileUnSatifyAndFail.open(dir+"/UnSatifyAndFail.txt", ios::out);

 	// 检查文件是否成功打开
 	if (!fileSatifyAndSucc.is_open() || !fileSatifyAndFail.is_open() || !fileUnSatifyAndSucc.is_open() || !fileUnSatifyAndFail.is_open()) 
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
/* 处理第一种情况 符合结构 满足距离要求*/

void HandleCaseSatifyAndSucc(string x,string y,vector<vector<int>> &dp,int d,ofstream &myfile)
{
	// /* 求所有的公共子序列 */
	set<string> SetSubStr;
	string s;
	getAllCommonSubStr(x,y,dp,SetSubStr);
	set<string>::iterator beg = SetSubStr.begin();
	int cnt=0;
	int targenlen = removeSpace(x).length()-d;
	for( ; beg!=SetSubStr.end(); ++beg)
	{
		if((*beg).length()==targenlen)
		{
			cnt+=1;
			if(NEED_OUTPUT_ALL_SUBSTR)
				myfile <<"   -"<<to_string(cnt)<<": "<< *beg <<endl;
		}
	}
	myfile<< x<<"  "<< y <<"  YES  " <<"["+to_string(d)+"]   -" <<cnt<<endl;

	
	myfile<<endl;
 
	//cout<< s+"  \"YES\" ["+to_string(d)+"]\n" ;
}

/* 处理第一种情况 符合结构 不满足距离要求*/
void HandleCaseSatifyAndFail(string s,int d,ofstream &myfile)
{
	myfile<< s+"  \"No\" ["+to_string(d)+"]\n" ;
	//cout<< s+"  \"NO\" ["+to_string(d)+"]\n" ;
}
/* 第三种情况，不符合结构 也满足距离要求 */
void HandleCaseUnSatifyAndSucc(string x,string y,vector<vector<int>> &dp,int d,ofstream &myfile)
{
	/* 求所有的最长公共子序列 */
	set<string> SetSubStr;
	string s;
	getAllCommonSubStr(x,y,dp,SetSubStr);
	set<string>::iterator beg = SetSubStr.begin();
	int cnt=0;
	int targenlen = removeSpace(x).length()-d;
	for( ; beg!=SetSubStr.end(); ++beg)
	{
		if((*beg).length()==targenlen)
		{
			cnt+=1;
			if(NEED_OUTPUT_ALL_SUBSTR)
				myfile <<"   -"<<to_string(cnt)<<": "<< *beg <<endl;
		}
	}
	myfile<< x<<"  "<< y <<"  No  " <<"["+to_string(d)+"]   -" <<cnt<<endl;
	myfile<<endl;
 
	//cout<< s+"  \"YES\" ["+to_string(d)+"]\n" ;
}

/* 第四种情况，不符合结构 且不满足距离要求*/
void HandleCaseUnSatifyAndFail(string s,int d,ofstream &myfile)
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

/*

 函数参数分析
 t总共的错误个数
 idx： 要构造的段 从1开始


 函数要做的事情：
 1.按照给定的开始符号，去完成序列的构造  for(int i=xxx) 也就是将本段的长度构造为i
 2.上次递归会给出第一个符号以及此时是否翻转，因此我们这里是需要针对每个i
    - 确定当前长度是 下一个符号分别为0 1时的逆转情况
	- 根据此时本段的left和right逆转情况 修正checkCons


上一个段是否满足要求，在上一个段给出本段的起始符号的时候就已经确定了！！

*/


void ClassfiyAndCompeteSubsequence(int t,int idx,string nows,char beginc,bool leftreverse,bool checkcons)
{
	/* 已经把所有的段都赋值好了 根据序列计算出互补序列*/
	if(idx == 2*t)
	{
		string x=nows,y = FindcoupleOfX(nows);
		string sx = removeSpace(x),sy=removeSpace(y);
		int n = sx.length();
		vector<vector<int>> dp(n+1,vector<int>(n+1,0));
		int d = n - lcs(n,n,sx,sy,dp);   /* 满足结构且距离等于t   在预期内的成功*/

		if ( checkcons && d >= t)
		{
			HandleCaseSatifyAndSucc(x,y,dp,d,fileSatifyAndSucc);
		}
		else if(checkcons)   /* 满足描述结构，但距离小于t  未达预期 + 失败 */
		{
			HandleCaseSatifyAndFail(x,d,fileSatifyAndFail);
		}
		else if (d >=  t)  /* 虽然没有满足结构 但仍人有足够的距离*/
		{
			HandleCaseUnSatifyAndSucc(x,y,dp,d,fileUnSatifyAndSucc);
		}
		else   /* 没有满足结构，且距离小于t  在预期内的失败*/
		{
			HandleCaseUnSatifyAndFail(x,d,fileUnSatifyAndFail);
		}
		return;
	}

/* 根据当前段的第一个符号+当前段的长度+左边是不是翻转+下一个符号取值来分析出是完全交替还是0交替*/
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
		char barbeginc = (beginc=='0'?'1':'0');   //本段开头字符相反的字符
		bool rightreserved;     //右边是否翻转
		bool newCheckCons;        //增加了该段之后判断是不是仍然满足要求的条件
		
	    /* 下一段第一个符号和本段第一个一样： 根据本段长度，可以判断右边是否反转 再结合左边翻转情况就知道是不是满足结构要求*/
		ns = gen_next_string(nows,beginc,i);
		rightreserved = i %2;
		if(checkcons)    /* 这里判断长度的时候 需要判断的是前一段的长度，而是目前在追加的段 idx 因为只有下一段确定了开始字符才能知道上一段到底是单反赚还是什么情况*/
			newCheckCons =  (i > min(idx,2*t-idx))  || (leftreverse == rightreserved);
		else
			newCheckCons = false; 
		// if(nows == "01 01 01 "){
		// 	cout<<"(seglen >min(idx,2*t-idx)) :"<<(i > min(idx,2*t-idx)) <<"("<<i<<","<< min(idx,2*t-idx)<<")"
		// 	<<"   leftreverse == rightreserved:"<<(leftreverse == rightreserved)<<" l:"<<leftreverse <<"  r:"<<rightreserved<<endl;

		// 	cout<<"idx="<<   idx <<"    same:  "<< beginc<<"   "<<" len: " <<i
		// 	<<"   "<<leftreverse<<"     "<<rightreserved<<"    " <<newCheckCons
		// 	<<"   "<<nows<<"   "<<ns<<" "<<beginc
		// 	<<endl;
		// }
		ClassfiyAndCompeteSubsequence(t,idx+1,ns,beginc,rightreserved,newCheckCons) ;     //ps:  && i<=(min(idx,2*t-idx))  修改为  && (i<=(min(idx,2*t-idx)))    &&比<=的优先级高
	
		
		/* 下一段第一个符号和本段相反 ,此时如果本段长度为偶数则为反转  101 01xxx    1010 01xxx*/
		rightreserved = (i-1)%2;
		if(checkcons)
			newCheckCons = (i > min(idx,2*t-idx)) || (leftreverse == rightreserved);
		else
			newCheckCons = false; 
		// if(nows == "01 01 01 "){
		// 	cout<<"(seglen >min(idx,2*t-idx)) :"<<(i >min(idx,2*t-idx)) <<"("<<i<<","<< min(idx,2*t-idx)<<")"
		// 	<<"   leftreverse == rightreserved:"<<(leftreverse == rightreserved)<<" l:"<<leftreverse <<"  r:"<<rightreserved<<endl;

		// 	cout<<"idx="<<   idx <<"    diff:  "<< barbeginc<<"   "<<" len: " <<i
		// 	<<"   "<<leftreverse<<"     "<<rightreserved<<"    " <<newCheckCons
		// 	<<"   "<<nows<<"   "<<ns <<" "<<barbeginc
		// 	<<endl;
		// }
		ClassfiyAndCompeteSubsequence(t,idx+1,ns,barbeginc,rightreserved,newCheckCons) ;

	}
}

/*
01 01 01 101 10   "YES" [2]
01 01 01 101 010   "YES" [2]
01 010 10 01 10   "YES" [2]
*/



/* 相较于前面统计了全部的结果，这里我们只统计边界的情况
怎么理解边界的情况？
对于第i段 完全翻转的话 只统计长度为2的情况和长度为3的情况
单翻转的话统计长度为idx 和idx+1的情况
理论上应该是idx 不成立，idx+1成立
*/
void LightlyClassfiyAndCompeteSubsequence(int t,int idx,string nows,char beginc,bool leftreverse,bool checkcons)
{
	/* 已经把所有的段都赋值好了 根据序列计算出互补序列*/
	if(idx == 2*t)
	{
		string x=nows,y = FindcoupleOfX(nows);
		string sx = removeSpace(x),sy=removeSpace(y);
		int n = sx.length();
		vector<vector<int>> dp(n+1,vector<int>(n+1,0));
		int d = n - lcs(n,n,sx,sy,dp);   /* 满足结构且距离等于t   在预期内的成功*/

		if ( checkcons && d >= t)
		{
			HandleCaseSatifyAndSucc(x,y,dp,d,fileSatifyAndSucc);
		}
		else if(checkcons)   /* 满足描述结构，但距离小于t  未达预期 + 失败 */
		{
			HandleCaseSatifyAndFail(x,d,fileSatifyAndFail);
		}
		else if (d >=  t)  /* 虽然没有满足结构 但仍人有足够的距离*/
		{
			HandleCaseUnSatifyAndSucc(x,y,dp,d,fileUnSatifyAndSucc);
		}
		else   /* 没有满足结构，且距离小于t  在预期内的失败*/
		{
			HandleCaseUnSatifyAndFail(x,d,fileUnSatifyAndFail);
		}
		return;
	}

/* 根据当前段的第一个符号+当前段的长度+左边是不是翻转+下一个符号取值来分析出是完全交替还是0交替*/

	    // 
		/* Case 1:长度为2 0翻转或完全翻转
		   目前是否仍然满足结构怎么求? 
		   原来不满足结构，现在一定不满足
		   原来满足+ 现在完全翻转  一定满足
		   原来满足+现在不完全翻转但长度大于等于要求 
		*/
		string ns;   //新的字符串
		int i=2;
		char barbeginc = (beginc=='0'?'1':'0');   //本段开头字符相反的字符
		bool rightreserved;     //右边是否翻转
		bool newCheckCons;        //增加了该段之后判断是不是仍然满足要求的条件

		char nextsegbegin;
		if(leftreverse)  //如果左边翻转了，这里也要翻转 那么就和第一位相反(因为长度是2，否则和第一位相同)
			nextsegbegin = barbeginc;
		else 
			nextsegbegin = beginc;
		ns = gen_next_string(nows,beginc,i);
		rightreserved = leftreverse; 
		newCheckCons = checkcons;
		if(nows == "01 010 "){
			cout<<"(seglen >min(idx,2*t-idx)) :"<<(i > min(idx,2*t-idx)) <<"("<<i<<","<< min(idx,2*t-idx)<<")"
			<<"   leftreverse == rightreserved:"<<(leftreverse == rightreserved)<<" l:"<<leftreverse <<"  r:"<<rightreserved<<endl;
			cout<<"idx="<<   idx <<"    len=2:  "<< beginc<<"   "<<" len: " <<i
			<<"   "<<leftreverse<<"     "<<rightreserved<<"    " <<newCheckCons
			<<"   "<<nows<<"   "<<ns<<" "<<nextsegbegin
			<<endl;
		}
		LightlyClassfiyAndCompeteSubsequence(t,idx+1,ns,nextsegbegin,rightreserved,newCheckCons) ;     //ps:  && i<=(min(idx,2*t-idx))  修改为  && (i<=(min(idx,2*t-idx)))    &&比<=的优先级高
	
		

		// Case 2-3: 长度分别为 min(idx,2t-idx) 和 min(idx,2t-idx) +1;  此时右边的翻转情况诗一致的 由左边的决定
		i = min(idx,2*t-idx);		
		if((leftreverse && (i%2==0)) || (!leftreverse && (i%2)==1)) //如果左边翻转了，且长度为奇，为了使得这里不翻转，下一个应该和开始符号相反；或者左边没翻转，这里
			nextsegbegin = beginc;
		else 
			nextsegbegin = barbeginc;

		ns = gen_next_string(nows,beginc,i);
		rightreserved = !leftreverse; 
		//1.长度为idx是 是不满足的
		newCheckCons = checkcons && false;
		if(nows == "01 "){
			cout<<"(seglen >min(idx,2*t-idx)) :"<<(i > min(idx,2*t-idx)) <<"("<<i<<","<< min(idx,2*t-idx)<<")"
			<<"   leftreverse == rightreserved:"<<(leftreverse == rightreserved)<<" l:"<<leftreverse <<"  r:"<<rightreserved<<endl;
			cout<<"idx="<<   idx <<"    len=2:  "<< beginc<<"   "<<" len: " <<i
			<<"   "<<leftreverse<<"     "<<rightreserved<<"    " <<newCheckCons
			<<"   "<<nows<<"   "<<ns<<" "<<nextsegbegin
			<<endl;
		}
		LightlyClassfiyAndCompeteSubsequence(t,idx+1,ns,nextsegbegin,rightreserved,newCheckCons) ;
		 
		// Case 3：
		i = min(idx,2*t-idx)+1;		
		if((leftreverse && (i%2==1)) || (!leftreverse && (i%2)==0)) //如果左边翻转了，且长度为奇，为了使得这里不翻转，下一个应该和开始符号相反；或者左边没翻转，这里
			nextsegbegin = barbeginc;
		else 
			nextsegbegin = beginc;

		ns = gen_next_string(nows,beginc,i);
		rightreserved = !leftreverse; 
		newCheckCons = checkcons && true;
		if(nows == "01 "){
			cout<<"(seglen >min(idx,2*t-idx)) :"<<(i > min(idx,2*t-idx)) <<"("<<i<<","<< min(idx,2*t-idx)<<")"
			<<"   leftreverse == rightreserved:"<<(leftreverse == rightreserved)<<" l:"<<leftreverse <<"  r:"<<rightreserved<<endl;
			cout<<"idx="<<   idx <<"    len=2:  "<< beginc<<"   "<<" len: " <<i
			<<"   "<<leftreverse<<"     "<<rightreserved<<"    " <<newCheckCons
			<<"   "<<nows<<"   "<<ns<<" "<<nextsegbegin
			<<endl;
		}
		LightlyClassfiyAndCompeteSubsequence(t,idx+1,ns,nextsegbegin,rightreserved,newCheckCons) ;
}
