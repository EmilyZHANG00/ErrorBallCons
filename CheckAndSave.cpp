#include<iostream>
#include<fstream>
#include "HandleFile.h"
using namespace std;



//void ClassfiyAndCompeteSubsequence(int t,int idx,string nows,char beginc,bool leftreverse,bool checkcons)

int  main()
{
	int t=3;
    string s="";
	if(!OpenAllFiles("../resdata/t-"+to_string(t)))
	return 0;

	ClassfiyAndCompeteSubsequence(t,1,s,'0',false,true);
}