#pragma once;

#include <time.h>
#include <vector>
#include <iostream>
using namespace std;

string int2string(int number);
int string2int(string str);
string llong2string(long long number);

string lowToUpper(string str);

double random(double start, double end);

string time2str(time_t const timer);  //时间戳转换为系统时间
time_t gettime();    //获得当前时间戳
string getHourMinute();  //获取时分
string getData();  //获取日期
long long getCurrentTime();  //获得当前时间，ms级

int bytesOfWord(const char firByte);
void splitLongString(
					 const char* pSrc,
					 char *pOut,
					 const int outLen,
					 const int words,
					 const char* split = "@!@" );

void splitToVector(const char* pSrc,
				   vector<std::string>& vec,
				   const char* pTok = "@!@");

string splitHeadString(const char *src, unsigned int head_len = 4, const char *endStr="..");

string ltrim(string str);   //去掉首空格
string rtrim(string str);   //去掉尾空格
string int2Coin(int coin);

string splitLongStringWithMoveLine(const char *src, unsigned int head_len = 4);