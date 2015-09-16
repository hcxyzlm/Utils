#include "utils/Util.h"
#include <locale.h> 
#include <string>
#include <cstdlib>

using namespace std;

#include <fcntl.h>
#include "errno.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/timeb.h>

#ifdef WIN32
#define snprintf _snprintf
#endif


string int2string(int number)
{
	char s[16];
	snprintf(s,sizeof(s) - 1,"%d",number);

	return (string)s;
}
int string2int(string str)
{
	return atoi(str.c_str());
}
string llong2string(long long number)
{
	char s[20];
	snprintf(s,sizeof(s) - 1,"%lld",number);

	return (string)s;
}


string lowToUpper(string str)
{
	int i=0;
	while (i<str.size())
	{
		if(str[i] >= 'a' && str[i] <= 'z')
			str[i] -= 0x20;
		i++;
	}
	
	return str;
}

double random(double start, double end)
{
	return start+(end-start)*rand()/(RAND_MAX + 1.0);
}

string time2str(time_t const timer)
{  
	struct tm *l=localtime(&timer);
	if (l == NULL)
	{
		return string("");
	}
	char buf[128];  
	snprintf(buf,sizeof(buf),"%04d-%02d-%02d %02d:%02d:%02d",l->tm_year+1900,l->tm_mon+1,l->tm_mday,l->tm_hour,l->tm_min,l->tm_sec);  
	string s(buf);  
	return s;  
}

time_t gettime()
{
	return time(0);
}

string getHourMinute()  //��ȡʱ��
{
	string ret = time2str(gettime());
	return ret.substr(11, 5);
}
string getData()  //��ȡ����
{
	string ret = time2str(gettime());
	return ret.substr(0, 10);
}
long long getCurrentTime()  //��õ�ǰʱ�䣬ms��
{
	timeb tp;
	ftime(&tp);
	return ((long long)(tp.time))*1000+tp.millitm;
}


int bytesOfWord(const char firByte)
{
	//�������
	int ret = 0;
	int ast = 0;

	//���ֽ��ַ���λ��0
	if( (firByte & (0x80)) == 0 )
	{
		return 1;
	}else
	{
		//�����ֽ�ͷ����1�ĸ���
		for(ast = 0; ast <= 7; ast++)
		{
			if( (firByte & (0x80 >> ast) ) != 0)
			{
				ret++;
			}else
			{
				break;
			}
		}
		return ret;
	}

	return 1;
}

//��ȡָ�����ȵ��ַ���������������ȣ�
//	pSrc: Դ�ַ���
//	pOut: ����ַ���
//outLen: ����ַ����ĳ���
// words: ���ٸ���λ�����ָ���(��ȫ���ַ��㣬����ַ���������)
void splitLongString(const char* pSrc, char *pOut, const int outLen, const int words, const char* split )
{
	memset(pOut, 0, outLen);

	char c = 0;
	float currWords = 0;
	int wordBytes = 0;

	char *pOutCurr = pOut;

	const char* splitor = split;
	int splitor_len = strlen(splitor);

	for(unsigned int i = 0; i < strlen(pSrc);)
	{
		c = *(pSrc + i);
		wordBytes = bytesOfWord(c);
		if((currWords + 0.5) >= words)
		{
			if( (pOutCurr - pOut) + splitor_len > outLen)
			{
				break;
			}
			memcpy(pOutCurr,splitor, splitor_len);
			pOutCurr += splitor_len;
			currWords = 0;
		}

		if( (pOutCurr - pOut) + wordBytes > outLen)
		{
			break;
		}

		memcpy(pOutCurr, pSrc + i, wordBytes);
		wordBytes >= 2 ? currWords += 1 : currWords += 0.5;
		pOutCurr += wordBytes;
		i += wordBytes;
	}
}

void splitToVector(const char* pSrc, vector<std::string>& vec, const char* pTok)
{
	int tokenLen = strlen(pTok);
	const char* pHead = NULL;
	const char* pNextPos = NULL;
	pHead = pSrc;
	char temp[256] = {0};
	while(1)
	{
		memset(temp, 0, sizeof(temp));
		pNextPos = strstr(pHead, pTok);
		//�����β
		if(pNextPos == NULL)
		{
			strcpy(temp, pHead);
			vec.push_back(std::string(temp));
			break;
		}
		//δ����β
		else
		{
			memcpy(temp, pHead, pNextPos - pHead);
			vec.push_back(std::string(temp));
			pHead = pNextPos + tokenLen;
		}
	}
}

string splitHeadString(const char *src, unsigned int head_len, const char *endStr)
{
	if(src == NULL) return " ";
	string retName = " ";
	char splitedStr[128] = {0};
	splitLongString(src, splitedStr, 128, head_len);
	vector<string> splitedVec;
	splitToVector(splitedStr, splitedVec);

	if(splitedVec.size() > 1)
	{
		retName = splitedVec.at(0) + endStr;
	}else
	{
		retName = string(src);
	}
	return retName;
}

string ltrim(string str)   //ȥ���׿ո�
{
	int endIndex = str.find_first_not_of(' ');
	
	if (endIndex>=0 && endIndex<str.length())        
	{
		return str.substr(endIndex, str.length()-endIndex);
	}
	return str;
}	

string rtrim(string str)   //ȥ��β�ո�
{
	int endIndex = str.find_last_not_of(' ');
	if (endIndex>=0 && endIndex<str.length())
	{
		return str.substr(0, str.length()-endIndex);
	}
	return str;
}

string int2Coin(int coin)
{
	string ret = "";
	vector<int> numDatas;

	if (coin<1000)
	{
		numDatas.push_back(coin);
	}
	else
	{
		while (coin/1000 > 0)
		{
			numDatas.push_back(coin%1000);
			coin /= 1000;
		}
		numDatas.push_back(coin);
	}
	for (int j=numDatas.size()-1;j>=0;j--)
	{
		if (j == numDatas.size()-1)
		{
			ret.append(int2string(numDatas[j]).c_str());
		}
		else
		{
			char toStr[8] = {0};
			if (numDatas[j]<10)
			{
				snprintf(toStr, sizeof(toStr)-1, "00%d", numDatas[j]);
			}
			else if (numDatas[j]<100)
			{
				snprintf(toStr, sizeof(toStr)-1, "0%d", numDatas[j]);
			}
			else
			{
				snprintf(toStr, sizeof(toStr)-1, "%d", numDatas[j]);
			}
			ret.append(",");
			ret.append(toStr);
		}
	}
	return ret;
}

std::string splitLongStringWithMoveLine( const char *src, unsigned int head_len /*= 4*/ )
{
	if(src == NULL) return "";
	string retName = "";
	char splitedStr[255] = {0};
	splitLongString(src, splitedStr, 255, head_len);
	vector<string> splitedVec;
	splitToVector(splitedStr, splitedVec);
	for (int i=0;i<splitedVec.size();i++)
	{
		retName+=splitedVec.at(i);
		if (i !=splitedVec.size()-1)
		{
			retName+="\n";
		}
	}
	return retName;
}
