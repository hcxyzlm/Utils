#ifndef __LangUtil_h__
#define __LangUtil_h__
#include <string>
#include <map>
using namespace std;

class LangUtil 
{
private:
//	hash_map<string, string> mapStr ;
public:
	LangUtil();
	LangUtil(string Language);
	void addFile(string fileName);
	string getStr(string name, char *format, ...);
	string getStr(string name);
	void setLanguage(std::string language);
private:
	string m_pLanguage;
	string replaceStr(string name, const string& des, const char* src);
	map<string, string> m_tipsMap;

};

extern LangUtil *g_pLangUtil;

#endif
