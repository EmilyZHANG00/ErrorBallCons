#include<iostream>
#include<fstream>
#include "HandleFile.h"
using namespace std;



//void ClassfiyAndCompeteSubsequence(int t,int idx,string nows,char beginc,bool leftreverse,bool checkcons)

int  main()
{
	int t=7;
	//NEED_OUTPUT_ALL_SUBSTR = true;                //是否要输出全部数据  t=4时执行全部数据 不到1h  800多MB；   t=5时不输出数据 3.5G
    string s="";
	if(!OpenAllFiles("../resdata/t-"+to_string(t)))
	return 0;

	/* 初始时上一段长度无所谓 因此这里设置成2t*/
	//ClassfiyAndCompeteSubsequence(t,1,s,'0',false,true);   // 探测所有情况
	LightlyClassfiyAndCompeteSubsequence(t,1,s,'0',false,true);  //仅仅探测边界情况；
}