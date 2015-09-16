#include "LangUtil.h"

#include "cocos2d.h"
USING_NS_CC;

#ifdef _WIN32
#include <io.h>
#include <direct.h>  
#else
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

#include <stdarg.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

LangUtil *g_pLangUtil = NULL;
LangUtil::LangUtil()
{
	addFile("tips.xml");
}

LangUtil::LangUtil(string Language)
{
	//char buffer[128]={0};
	setLanguage(Language);
}

void LangUtil::addFile(string fileName)
{
	CCLog("parse file: %s", fileName.c_str());
	m_tipsMap.clear();

	xmlDocPtr   pdoc = NULL;
    xmlNodePtr  proot = NULL;
    xmlNodePtr  curNode = NULL;
    xmlNodePtr  listNode = NULL;
  
	unsigned long nSize = 0;
	unsigned char* pBuffer = CCFileUtils::sharedFileUtils()->getFileData(fileName.c_str(), "rb", &nSize);

    pdoc = xmlParseMemory ((char*)pBuffer, nSize);
    
    if (pdoc == NULL)
    {
		CCLog("parse tips xml error");
        return;
    }
    
    // 获取 xml 文档对象的根节对象
    proot = xmlDocGetRootElement (pdoc);
    
    if (proot == NULL)
    {
        return;
    }
    
	curNode = proot->xmlChildrenNode;
	if (curNode != NULL)
	{
		while (curNode)
		{
			if (xmlStrcmp(curNode->name, BAD_CAST "tipstr")==0)
			{
				xmlChar *id = xmlGetProp(curNode, BAD_CAST "id");    // 获取属性值
				xmlChar *str = xmlGetProp(curNode, BAD_CAST "str");    // 获取属性值

				m_tipsMap[(char*)id] = (char*)str;

				xmlFree(id);
				xmlFree(str);
			}
			curNode = curNode->next;
		}
	}
    
    /* 关闭和清理 */
    xmlFreeDoc(pdoc);
    xmlCleanupParser();
}

string LangUtil::getStr(std::string name)
{
	return m_tipsMap[name];
}

string LangUtil::getStr(string name, char *format, ...)
{
	//format是和sprintf一样的方式,支持%s,%d,%f,%
	va_list argp;
	char *pNext ;
	char *pParam ;
	char buf[1024];
	int iValue;
	float fValue;
	int index = 1;

	string msg = m_tipsMap[name];

	va_start(argp, format);
	pNext = strchr(format, '%');
	while (pNext){
		memset(buf, 0, 1024);
		switch (*(pNext+1)){
			case 's':
				pParam = va_arg(argp, char*);
				sprintf(buf, "%s", pParam);
				break ;
			case 'd':
				iValue = va_arg(argp, int);
				sprintf(buf, "%d", iValue);
				break ;
			case 'f':
				fValue = va_arg(argp, float);
				sprintf(buf, "%f", fValue);
				break ;
			case 'S':
				break ;
			default:
				va_arg(argp,void*);
		}
		char indexBuf[8];
		memset(indexBuf, 0, sizeof(indexBuf));
		sprintf(indexBuf, "{%d}", index++);

		msg = replaceStr(msg, indexBuf, buf);
		pNext = pNext+2;
		pNext = strchr(pNext,'%');
	}
	va_end(argp);

	return msg ;
}

string LangUtil::replaceStr(string name, const string& des, const char* src)
{
	int pos = name.find(des);
	if (pos != string::npos)
	{
		name = name.replace(pos, des.length(), src);
	}
	return name;
}

void LangUtil::setLanguage(std::string language)
{
	char str[128];
	memset(str,0,sizeof(str));
	sprintf(str,"i18n/%s/tips.xml",language.c_str());
	if (access(str, 0) == -1)
	{

		memset(str,0,sizeof(str));
		sprintf(str,"i18n/%s/tips.xml","default");
	}
	//sprintf(buffer,"%s%s%s", "i18n/",Language.c_str(),"/tips.xml");
	addFile(str);
}
