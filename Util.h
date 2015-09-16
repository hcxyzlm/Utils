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

string time2str(time_t const timer);  //ʱ���ת��Ϊϵͳʱ��
time_t gettime();    //��õ�ǰʱ���
string getHourMinute();  //��ȡʱ��
string getData();  //��ȡ����
long long getCurrentTime();  //��õ�ǰʱ�䣬ms��

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

string ltrim(string str);   //ȥ���׿ո�
string rtrim(string str);   //ȥ��β�ո�
string int2Coin(int coin);

string splitLongStringWithMoveLine(const char *src, unsigned int head_len = 4);