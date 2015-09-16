#ifndef _H_INIFILE_H_
#define _H_INIFILE_H_

#include <string>
#include <iostream>
using namespace std;


class IniFile
{
public:
	// По
	class IniItem
	{
	public:
		IniItem();
		IniItem(const string& strItemName, const string& strItemValue);
		~IniItem();
		string m_strName;
		string m_strValue;
		IniItem*	m_pNextIniItem;
	};
	
	// ¶О
	class IniField
	{
	public:
		IniField();
		IniField(const string& strName);
		~IniField();
		void Release();
		IniItem* FindItem(const string& strItemName);
		IniItem* NewItem(const string& strItemName, const string& strItemValue);
		IniItem* SetItem(const string& item, const string& value);
		string m_strName;
		IniItem* m_pIniItemHead;
		IniItem* m_pIniItemTail;
		IniField* m_pNextIniField;
	};
public:
	IniFile();
	IniFile(const string& strName);
	int Open(const string& strName);
	void Release();
	IniItem* SetItem(const string& strFieldName, const string& strItemName, const string& strValue);
	string	GetItemString(const string& strFieldName, const string& strItemName);
	int		GetItemInt(const string& strFieldName, const string& strItemName);
	double	GetItemDouble(const string& strFieldName, const string& strItemName);
	~IniFile();
	void Show();
private:
	int Open();
	int Getline(FILE* fp, char* cBuff, int nBufLen, string& strLine);
	int Save();
	IniField* FindField(const string& strFieldName);
	IniItem* FindItem(const string& strFieldName, const string& strItemName);
	IniField* NewField(const string& strFiledName);
	IniItem* NewItem(const string& strFieldName, const string& strItemName, const string& strValue);
	string Trim(string s);
	bool		m_bModify;

	string m_strName;
	IniField* m_pIniFieldHead;
	IniField* m_pIniFieldTail;
};

#endif