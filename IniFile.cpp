#include "IniFile.h"
#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////////////////
IniFile::IniItem::IniItem()
: m_pNextIniItem(NULL)
{}

IniFile::IniItem::IniItem(const string& strName, const string& strValue)
: m_strName(strName), m_strValue(strValue), m_pNextIniItem(NULL)
{}

IniFile::IniItem::~IniItem()
{}
///////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////
IniFile::IniField::IniField()
: m_pIniItemHead(NULL), m_pIniItemTail(NULL), m_pNextIniField(NULL)
{}

IniFile::IniField::IniField(const string& strName)
: m_pIniItemHead(NULL), m_pIniItemTail(NULL), m_pNextIniField(NULL),
	m_strName(strName)
{}


void IniFile::IniField::Release()
{
	IniFile::IniItem* pIniItem = m_pIniItemHead, *pIniItemTemp;
	if(m_pIniItemHead)
		pIniItemTemp = m_pIniItemHead->m_pNextIniItem;
	while(pIniItem)
	{
		delete pIniItem;
		pIniItem = pIniItemTemp;
		if(pIniItem)
			pIniItemTemp = pIniItem->m_pNextIniItem;
	}
}

IniFile::IniField::~IniField()
{
	Release();
}

IniFile::IniItem* IniFile::IniField::FindItem(const string& strItemName)
{
	IniItem* pIniItem = m_pIniItemHead;
	while(pIniItem)
	{
		if(pIniItem->m_strName == strItemName)
			return pIniItem;
		if(pIniItem)
			pIniItem = pIniItem->m_pNextIniItem;
	}
	return pIniItem;
}

IniFile::IniItem* IniFile::IniField::SetItem(const string& strItemName, const string& strItemValue)
{
	IniItem* pIniItem = FindItem(strItemName);
	if(pIniItem)
		pIniItem->m_strValue = strItemValue;
	else
		return NewItem(strItemName, strItemValue);
	return pIniItem;
}

IniFile::IniItem* IniFile::IniField::NewItem(const string& strItemName, const string& strItemValue)
{
	if(m_pIniItemTail)
	{
		m_pIniItemTail->m_pNextIniItem = new IniItem(strItemName, strItemValue);
		m_pIniItemTail = m_pIniItemTail->m_pNextIniItem;
	}
	else
	{
		m_pIniItemHead = new IniItem(strItemName, strItemValue);
		m_pIniItemTail = m_pIniItemHead;
	}
	return m_pIniItemTail;
}

///////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////
IniFile::IniField* IniFile::FindField(const string& strFieldName)
{
	IniField* pIniField = m_pIniFieldHead;
	while(pIniField)
	{
		if(pIniField->m_strName == strFieldName)
			return pIniField;
		if(pIniField)
			pIniField = pIniField->m_pNextIniField;
	}
	return pIniField;
}

IniFile::IniField* IniFile::NewField(const string& strFieldName)
{
	if(m_pIniFieldHead)
	{
		m_pIniFieldTail->m_pNextIniField = new IniField(strFieldName);
		m_pIniFieldTail = m_pIniFieldTail->m_pNextIniField;
	}
	else
	{
		m_pIniFieldHead = new IniField(strFieldName);
		m_pIniFieldTail = m_pIniFieldHead;
	}
	return m_pIniFieldTail;
}

IniFile::IniItem* IniFile::FindItem(const string& strFieldName, const string& strItemName)
{
	IniItem* pIniItem = NULL;
	IniField* pIniField = FindField(strFieldName);
	if(pIniField)
		pIniItem = pIniField->FindItem(strItemName);
	return pIniItem;
}

IniFile::IniFile()
: m_pIniFieldHead(NULL), m_pIniFieldTail(NULL), m_bModify(NULL)
{}

IniFile::IniFile(const string& strName)
: m_strName(strName), m_pIniFieldHead(NULL), m_pIniFieldTail(NULL), m_bModify(NULL)
{
	Open(m_strName);
}

IniFile::~IniFile()
{
	if(m_bModify)
		Save();
	Release();
}

int IniFile::Open(const string& strName)
{
	Release();
	m_strName = strName;
	return Open();
}

void IniFile::Release()
{
	IniField* pIniField = m_pIniFieldHead, *pIniFieldTemp;
	if(m_pIniFieldHead)
		pIniFieldTemp = m_pIniFieldHead->m_pNextIniField;
	while(pIniField)
	{
		delete pIniField;
		pIniField = pIniFieldTemp;
		if(pIniField)
			pIniFieldTemp = pIniField->m_pNextIniField;		
	}
}

int IniFile::Open()
{
	// 按行读文件
	FILE * fp;   
	fp = fopen(m_strName.c_str(), "r");   
	if(!fp)
	{
		fp = fopen(m_strName.c_str(), "w+");
		if(!fp)
			return -1;
	}
	
	string	strLine, strFiled;
	IniField* pIniField = NULL;
	IniItem* pIniItem = NULL;
	char cBuff[512] = {0};
	while(Getline(fp, cBuff, 512, strLine))
	{
		// 取消该行的首尾的空格
		Trim(strLine);
		if(strLine.length() <= 0)
			continue;
		if(strLine[0] == ';')				// 是注释
			continue;
		else if(strLine[0] == '[' && strLine[strLine.length() - 1] == ']')	// 是段
		{
			strFiled = strLine.substr(1, strLine.length() - 2);
			printf("[%s]\n", strFiled.c_str());
			pIniField = FindField(strFiled);
			// 查找是否已经添加该段
			if(!pIniField)		// 没有该段
				pIniField = NewField(strFiled);
			else
				continue;
		}
		else	// 内容
		{
			int nFind = strLine.find('=');
			if(nFind == string::npos || !pIniField)
				continue;
			else
			{
				printf("%s = %s\n", Trim(strLine.substr(0, nFind -1)).c_str(), 
					Trim(strLine.substr(nFind + 1, strLine.length())).c_str());
				pIniField->SetItem(Trim(strLine.substr(0, nFind -1)), 
					Trim(strLine.substr(nFind + 1, strLine.length())));	
			}
		}
	}
	fclose(fp);
	return 0;
}

IniFile::IniItem* IniFile::SetItem(const string& strFieldName, const string& strItemName, const string& strValue)
{
	IniField* pIniField = FindField(strFieldName);
	m_bModify = true;
	if(pIniField)
		return pIniField->SetItem(strItemName, strValue);
	else
		return NewItem(strFieldName, strItemName, strValue);
	return NULL;
}

IniFile::IniItem* IniFile::NewItem(const string& strFieldName, const string& strItemName, const string& strValue)
{	
	return NewField(strFieldName)->SetItem(strItemName, strValue);
}


string	IniFile::GetItemString(const string& strFieldName, const string& strItemName)
{
	IniItem* pIniItem = FindItem(strFieldName, strItemName);
	if(pIniItem)
		return pIniItem->m_strValue;
	return "";
}

int	IniFile::GetItemInt(const string& strFieldName, const string& strItemName)
{
	return atoi(GetItemString(strFieldName, strItemName).c_str());
}


double IniFile::GetItemDouble(const string& strFieldName, const string& strItemName)
{
	return atof(GetItemString(strFieldName, strItemName).c_str());
}

int IniFile::Getline(FILE* fp, char* cBuff, int nBufLen, string& strLine)
{
	char* pStr;
	strLine = "";
	while(1)
	{
		pStr = fgets(cBuff, nBufLen - 1, fp);
		if(pStr)
		{
			strLine += pStr;
			if(strlen(pStr) != nBufLen - 1)
				return strLine.length();
		}
		else
			return strLine.length();
	}
	return strLine.length();
}

int IniFile::Save()
{
	FILE* pf = fopen(m_strName.c_str(), "w");
	if(!pf)
		return -1;

	IniField* pIniField = m_pIniFieldHead;
	IniItem* pIniItem = NULL;
	while(pIniField)
	{
		fwrite("[", 1, 1, pf);
		fwrite(pIniField->m_strName.c_str(), 1, pIniField->m_strName.length(), pf);
		fwrite("]\n", 1, 2, pf);

		pIniItem = pIniField->m_pIniItemHead;
		while(pIniItem)
		{
			fwrite(pIniItem->m_strName.c_str(), 1, pIniItem->m_strName.length(), pf);
			fwrite(" = ", 1, 3, pf);
			fwrite(pIniItem->m_strValue.c_str(), 1, pIniItem->m_strValue.length(), pf);
			fwrite("\n", 1, 1, pf);
			pIniItem = pIniItem->m_pNextIniItem;
		}
		fwrite("\n", 1, 1, pf);
		pIniField = pIniField->m_pNextIniField;
	}
	fclose(pf);
	return 0;
}

string IniFile::Trim(string s)
{
	if (s.empty()) 
		return s;
	
	s.erase(0,s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
	s.erase(s.find_last_not_of("\n") + 1);
	
	return s;
}