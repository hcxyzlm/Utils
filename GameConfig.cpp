#include "GameConfig.h"
#include "cocos2d.h"
USING_NS_CC;


#define XML_SAFE_PARSE_INT(retInt, pXmlChar, CharLog)		if (pXmlChar!=NULL) {retInt = atoi((char*)pXmlChar);} else {CCLog("xml parse failed : %s", CharLog); retInt = -1;}
#define XML_SAFE_PARSE_STRING(retString, pXmlChar, CharLog)	if (pXmlChar!=NULL) {retString = string((char*)pXmlChar);} else {CCLog("xml parse failed : %s", CharLog); retString = "";}
#define XML_SAFE_PARSE_FLOAT(retInt, pXmlChar, CharLog)		if (pXmlChar!=NULL) {retInt = atof((char*)pXmlChar);} else {CCLog("xml parse failed : %s", CharLog); retInt = -1;}

GameConfig *g_pGameConfig = NULL;
GameConfig::GameConfig()
{
	m_debug.isDebug = false;
	parseConfig();
}

GameConfig::~GameConfig()
{

}

int GameConfig::getGameID()    //�����ϷID
{
	return m_gameID;
}

ProtectGiveConf GameConfig::getProptectInfo()
{
	return m_protectGive;
}

string GameConfig::getPlatLoginUrl()   //��ȡƽ̨��¼��֤url
{
	return m_webUrls["loginurl"];
}

string GameConfig::getServerListUtl()  //��ȡserverlist url
{
	return m_webUrls["serverurl"];
}

string GameConfig::getVerUpdateUrl()   //��ȡ�汾������֤url
{
	return m_webUrls["updateurl"];
}

string GameConfig::getModifyUserInfoUrl()  //��ȡ�û���Ϣ�޸�url
{
	return m_webUrls["modifyurl"];
}

string GameConfig::getWebUrl(string urlKey)
{
	return m_webUrls[urlKey];
}

string  GameConfig::getGameConfigUrl()  //��ȡ�����Ϸ������Ϣ��url
{
	return m_webUrls["gameconfigurl"];
}

string GameConfig::getExchangeUrl()  //��ȡ���ф��һ�url
{
	return m_webUrls["exchangeurl"];
}

string GameConfig::getNoticeUrl()  //��ȡ����һ�url
{
	return m_webUrls["noticeurl"];
}

string GameConfig::getMoreGameUrl()  //��ȡ������Ϸurl
{	
	return m_webUrls["moregameurl"];
}

string GameConfig::getRecommondUrl()   //��ȡ�Ƽ�serverid�ӿ�
{
	return m_webUrls["recommondurl"];
}

string GameConfig::getGameXmlInfo()  //��ȡ����xml�����ļ�
{
	return m_gameConfig;
}

LevelInfo GameConfig::getLevel(int point)
{
	LevelInfo level;
	if (m_levelInfos.size() <=0)
	{
		return level;
	}
	
	for(int i=0; i<m_levelInfos.size(); i++)
	{
		if (point < m_levelInfos[i].point)
		{
			level.point = m_levelInfos[i].point;
			level.level = m_levelInfos[i].level;
			level.levelname = m_levelInfos[i].levelname;
			return level;
		}
	}

	return m_levelInfos[m_levelInfos.size()-1];;
}

bool GameConfig::parseGameConfig(char *pBuffer, unsigned long nSize)  //����������
{
	m_regionInfos.clear();
	m_tableConfigs.clear();
	m_goodInfos.clear();
	m_servers.m_shardVec.clear();
	m_resVersion.clear();
	m_vipInfos.clear();
	m_mysShops.clear();
	m_switchs.clear();

	xmlDocPtr   pdoc = NULL;
	xmlNodePtr  proot = NULL;
	xmlNodePtr  curNode = NULL;

	m_gameConfig = string(pBuffer, nSize);

	pdoc = xmlParseMemory ((char*)pBuffer, nSize);

	if (pdoc == NULL)
	{
		return false;
	}

	// ��ȡ xml �ĵ�����ĸ��ڶ���
	proot = xmlDocGetRootElement (pdoc);

	if (proot == NULL)
	{
		return false;
	}

	curNode = proot->xmlChildrenNode;
	if (!curNode)
	{
		return false;
	}

	while (curNode != NULL)
	{
		if (xmlStrcmp(curNode->name, BAD_CAST "webs")==0)
		{
			parseWebs(curNode);
		}
		else if (xmlStrcmp(curNode->name, BAD_CAST "regions")==0)
		{
			parseRegions(curNode);
		}
		else if (xmlStrcmp(curNode->name, BAD_CAST "goods")==0)
		{
			parseGoods(curNode);
		}
		else if (xmlStrcmp(curNode->name, BAD_CAST "scenes")==0)
		{
			parseScenes(curNode);
		}
		else if (xmlStrcmp(curNode->name, BAD_CAST "servers")==0)
		{
			parseServerList(curNode);
		}
		else if (xmlStrcmp(curNode->name, BAD_CAST "givecoins")==0)
		{
			parseGaveCoin(curNode);
		}
		else if (xmlStrcmp(curNode->name, BAD_CAST "resources")==0)
		{
			parseResVersion(curNode);
		}
		else if (xmlStrcmp(curNode->name, BAD_CAST "vips")==0)
		{
			parseVip(curNode);
		}
		else if (xmlStrcmp(curNode->name, BAD_CAST "mysshop")==0)
		{
			parseMysShop(curNode);
		}
		else if (xmlStrcmp(curNode->name, BAD_CAST "switchs")==0)
		{
			parseSwitchs(curNode);
		}
		else if (xmlStrcmp(curNode->name, BAD_CAST "Languages")==0)
		{
			parseLanguages(curNode);
		}
		else if (xmlStrcmp(curNode->name, BAD_CAST "commondatas")==0)
		{
			parseCommonData(curNode);
		}
		curNode = curNode->next;
	}

	/* �رպ����� */
	xmlFreeDoc(pdoc);
	xmlCleanupParser();

	return true;
}

void GameConfig::parseConfig()
{
	m_regionInfos.clear();
	m_tableConfigs.clear();
	m_goodInfos.clear();
	m_levelInfos.clear();
	m_webUrls.clear();
	m_servers.m_shardVec.clear();
	m_resVersion.clear();
	m_vipInfos.clear();
	m_mysShops.clear();
	m_switchs.clear();

	xmlDocPtr   pdoc = NULL;
    xmlNodePtr  proot = NULL;
    xmlNodePtr  curNode = NULL;
    
	unsigned long nSize = 0;
	unsigned char* pBuffer = CCFileUtils::sharedFileUtils()->getFileData("game_config.xml", "rb", &nSize);

    pdoc = xmlParseMemory ((char*)pBuffer, nSize);
    
    if (pdoc == NULL)
    {
        return;
    }
    
    // ��ȡ xml �ĵ�����ĸ��ڶ���
    proot = xmlDocGetRootElement (pdoc);
    
    if (proot == NULL)
    {
        return;
    }
    
	curNode = proot->xmlChildrenNode;
	while (curNode != NULL)
	{
		if (xmlStrcmp(curNode->name, BAD_CAST "game")==0)
		{
			parseGameId(curNode);
		}
		else if (xmlStrcmp(curNode->name, BAD_CAST "levels")==0)
		{
			parseLevelInfo(curNode);
		}
		else if (xmlStrcmp(curNode->name, BAD_CAST "webs")==0)
		{
			parseWebs(curNode);
		}
		else if (xmlStrcmp(curNode->name, BAD_CAST "debug")==0)
		{
			parseDebug(curNode);
		}
		curNode = curNode->next;
	}
    
    /* �رպ����� */
    xmlFreeDoc(pdoc);
    xmlCleanupParser();
}

void GameConfig::parseGameId(xmlNodePtr node)
{
	xmlChar *gameid = xmlGetProp(node, BAD_CAST "gameid");    // ��ȡ����ֵ
	xmlChar *confversion = xmlGetProp(node, BAD_CAST "confversion");    // ��ȡ����ֵ

	XML_SAFE_PARSE_INT(m_gameID, gameid, "gameid");
	XML_SAFE_PARSE_INT(m_configVersion, confversion, "confversion");
	
	xmlFree(gameid);
	xmlFree(confversion);
}

void GameConfig::parseServerList(xmlNodePtr node)
{
	xmlNodePtr listNode = NULL;
	listNode = node->xmlChildrenNode;
	while (listNode)
	{
		if (xmlStrcmp(listNode->name, BAD_CAST "server")==0)
		{
			xmlChar *serverid = xmlGetProp(listNode, BAD_CAST "serverid");    // ��ȡ����ֵ
			xmlChar *servername = xmlGetProp(listNode, BAD_CAST "servername");    // ��ȡ����ֵ
			xmlChar *serverip = xmlGetProp(listNode, BAD_CAST "serverip");    // ��ȡ����ֵ
			xmlChar *serverport = xmlGetProp(listNode, BAD_CAST "serverport");    // ��ȡ����ֵ

			ServerShard server;
			XML_SAFE_PARSE_INT(server.serverid, serverid, "serverid");
			XML_SAFE_PARSE_INT(server.serverport, serverport, "serverport");
			XML_SAFE_PARSE_STRING(server.servername, servername, "servername");
			XML_SAFE_PARSE_STRING(server.serverip, serverip, "serverip");

			xmlFree(serverid);
			xmlFree(servername);
			xmlFree(serverip);
			xmlFree(serverport);

			m_servers.m_shardVec.push_back(server);
		}
		listNode = listNode->next;
	}
}

void GameConfig::parseGaveCoin(xmlNodePtr node)
{
	xmlNodePtr listNode = NULL;
	listNode = node->xmlChildrenNode;
	while (listNode)
	{
		if (xmlStrcmp(listNode->name, BAD_CAST "protect")==0)
		{
			parseProtect(listNode);
		}
		else if (xmlStrcmp(listNode->name, BAD_CAST "logincoins")==0)
		{
			parseLoginGive(listNode);
		}
		else if (xmlStrcmp(listNode->name, BAD_CAST "playcoins")==0)
		{
			parsePlayGive(listNode);
		}
		else if (xmlStrcmp(listNode->name, BAD_CAST "timecoins")==0)
		{
			parseTimeGive(listNode);
		}
		listNode = listNode->next;
	}
}

void GameConfig::parseProtect(xmlNodePtr node)
{
	xmlChar *lowcoin = xmlGetProp(node, BAD_CAST "lowcoin");    // ��ȡ����ֵ
	xmlChar *givecoin = xmlGetProp(node, BAD_CAST "givecoin");    // ��ȡ����ֵ
	xmlChar *count = xmlGetProp(node, BAD_CAST "count");    // ��ȡ����ֵ

	XML_SAFE_PARSE_INT(m_protectGive.lowcoin, lowcoin, "lowcoin");
	XML_SAFE_PARSE_INT(m_protectGive.givecoin, givecoin, "givecoin");
	XML_SAFE_PARSE_INT(m_protectGive.count, count, "count");

	xmlFree(lowcoin);
	xmlFree(givecoin);
	xmlFree(count);
}

void GameConfig::parseLoginGive(xmlNodePtr node)
{
	xmlNodePtr listNode = NULL;
	listNode = node->xmlChildrenNode;
	while (listNode)
	{
		if (xmlStrcmp(listNode->name, BAD_CAST "logincoin")==0)
		{
			xmlChar *day = xmlGetProp(listNode, BAD_CAST "day");    // ��ȡ����ֵ
			xmlChar *coin = xmlGetProp(listNode, BAD_CAST "coin");    // ��ȡ����ֵ
			xmlChar *fare = xmlGetProp(listNode, BAD_CAST "fare");    // ��ȡ����ֵ

			LoginGiveConf loginGive;
			XML_SAFE_PARSE_INT(loginGive.day, day, "day");
			XML_SAFE_PARSE_INT(loginGive.coin, coin, "coin");
			XML_SAFE_PARSE_INT(loginGive.fare, fare, "fare");

			xmlFree(day);
			xmlFree(coin);
			xmlFree(fare);

			m_loginGive.push_back(loginGive);
		}
		listNode = listNode->next;
	}
}

void GameConfig::parsePlayGive(xmlNodePtr node)
{
	xmlNodePtr listNode = NULL;
	listNode = node->xmlChildrenNode;
	while (listNode)
	{
		if (xmlStrcmp(listNode->name, BAD_CAST "playcoin")==0)
		{
			xmlChar *senceid = xmlGetProp(listNode, BAD_CAST "senceid");    // ��ȡ����ֵ
			
			PlayGiveConf playGive;
			XML_SAFE_PARSE_INT(playGive.scenceid, senceid, "senceid");
			xmlFree(senceid);

			playGive.sencePlayGives = parseSencePlayGive(listNode);

			m_playGive.push_back(playGive);
		}
		listNode = listNode->next;
	}
}

vector<SencePlayGive> GameConfig::parseSencePlayGive(xmlNodePtr node)
{
	vector<SencePlayGive> sencePlayGive;
	xmlNodePtr listNode = NULL;
	listNode = node->xmlChildrenNode;
	while (listNode)
	{
		if (xmlStrcmp(listNode->name, BAD_CAST "give")==0)
		{
			xmlChar *playcount = xmlGetProp(listNode, BAD_CAST "playcount");    // ��ȡ����ֵ
			xmlChar *coin = xmlGetProp(listNode, BAD_CAST "coin");    // ��ȡ����ֵ
			xmlChar *fare = xmlGetProp(listNode, BAD_CAST "fare");    // ��ȡ����ֵ

			SencePlayGive senceGive;
			XML_SAFE_PARSE_INT(senceGive.playcount, playcount, "playcount");
			XML_SAFE_PARSE_INT(senceGive.coin, coin, "coin");
			XML_SAFE_PARSE_INT(senceGive.fare, fare, "fare");

			xmlFree(playcount);
			xmlFree(coin);
			xmlFree(fare);

			sencePlayGive.push_back(senceGive);
		}
		listNode = listNode->next;
	}
	return sencePlayGive;
}

void GameConfig::parseTimeGive(xmlNodePtr node)
{
	xmlNodePtr listNode = NULL;
	listNode = node->xmlChildrenNode;
	while (listNode)
	{
		if (xmlStrcmp(listNode->name, BAD_CAST "timecoin")==0)
		{
			xmlChar *time = xmlGetProp(listNode, BAD_CAST "time");    // ��ȡ����ֵ
			xmlChar *coin = xmlGetProp(listNode, BAD_CAST "coin");    // ��ȡ����ֵ
			xmlChar *fare = xmlGetProp(listNode, BAD_CAST "fare");    // ��ȡ����ֵ

			TimeGivecConf timeGive;
			XML_SAFE_PARSE_INT(timeGive.time, time, "time");
			XML_SAFE_PARSE_INT(timeGive.coin, coin, "coin");
			XML_SAFE_PARSE_INT(timeGive.fare, fare, "fare");

			xmlFree(time);
			xmlFree(coin);
			xmlFree(fare);

			m_timeGive.push_back(timeGive);
		}
		listNode = listNode->next;
	}
}

void GameConfig::parseWebs(xmlNodePtr node)
{
	xmlNodePtr listNode = NULL;
	listNode = node->xmlChildrenNode;
	while (listNode)
	{
		if (xmlStrcmp(listNode->name, BAD_CAST "web")==0)
		{
			xmlChar *webkey = xmlGetProp(listNode, BAD_CAST "webkey");    // ��ȡ����ֵ
			xmlChar *weburl = xmlGetProp(listNode, BAD_CAST "weburl");    // ��ȡ����ֵ

			string strKey;
			string strUrl;

			XML_SAFE_PARSE_STRING(strKey, webkey, "webkey");
			XML_SAFE_PARSE_STRING(strUrl, weburl, "weburl");

			xmlFree(webkey);
			xmlFree(weburl);

			m_webUrls[strKey] = strUrl;
		}
		listNode = listNode->next;
	}
}

void GameConfig::parseResVersion(xmlNodePtr node)
{
	xmlNodePtr listNode = NULL;
	listNode = node->xmlChildrenNode;
	while (listNode)
	{
		if (xmlStrcmp(listNode->name, BAD_CAST "resource")==0)
		{
			xmlChar *module = xmlGetProp(listNode, BAD_CAST "module");    // ��ȡ����ֵ
			xmlChar *resversion = xmlGetProp(listNode, BAD_CAST "resversion");    // ��ȡ����ֵ

			string strModule;
			int strResVersion;

			XML_SAFE_PARSE_STRING(strModule, module, "module");
			XML_SAFE_PARSE_INT(strResVersion, resversion, "ersversion");

			xmlFree(module);
			xmlFree(resversion);

			m_resVersion[strModule] = strResVersion;
		}
		listNode = listNode->next;
	}
}

void GameConfig::parseVip(xmlNodePtr node)
{
	xmlNodePtr listNode = NULL;
	listNode = node->xmlChildrenNode;
	while (listNode)
	{
		if (xmlStrcmp(listNode->name, BAD_CAST "vip")==0)
		{
			xmlChar *level = xmlGetProp(listNode, BAD_CAST "level");    // ��ȡ����ֵ
			xmlChar *toprecharge = xmlGetProp(listNode, BAD_CAST "toprecharge");    // ��ȡ����ֵ
			xmlChar *givecoin = xmlGetProp(listNode, BAD_CAST "givecoin");    // ��ȡ����ֵ
			xmlChar *goodid = xmlGetProp(listNode, BAD_CAST "goodid");    // ��ȡ����ֵ

			
			VipInfo vipInfo;
			XML_SAFE_PARSE_INT(vipInfo.level, level, "level");
			XML_SAFE_PARSE_INT(vipInfo.topRechage, toprecharge, "toprecharge");
			XML_SAFE_PARSE_INT(vipInfo.giveCoin, givecoin, "givecoin");
			XML_SAFE_PARSE_INT(vipInfo.goodId, goodid, "goodid");

			xmlFree(level);
			xmlFree(toprecharge);
			xmlFree(givecoin);
			xmlFree(goodid);

			m_vipInfos[vipInfo.level] = vipInfo;
		}
		listNode = listNode->next;
	}
}

void GameConfig::parseMysShop(xmlNodePtr node)
{
	xmlChar *channel = xmlGetProp(node, BAD_CAST "channel");    // ��ȡ����ֵ
	string strChannel;
	XML_SAFE_PARSE_STRING(strChannel, channel, "channel");

	xmlNodePtr listNode = NULL;
	listNode = node->xmlChildrenNode;
	vector<MysShop> shopVec;
	while (listNode)
	{
		if (xmlStrcmp(listNode->name, BAD_CAST "good")==0)
		{
			xmlChar *goodid = xmlGetProp(listNode, BAD_CAST "goodid");    // ��ȡ����ֵ
			xmlChar *goodname = xmlGetProp(listNode, BAD_CAST "goodname");    // ��ȡ����ֵ
			xmlChar *preprice = xmlGetProp(listNode, BAD_CAST "preprice");    // ��ȡ����ֵ
			xmlChar *price = xmlGetProp(listNode, BAD_CAST "price");    // ��ȡ����ֵ
			xmlChar *desc = xmlGetProp(listNode, BAD_CAST "desc");    // ��ȡ����ֵ
			xmlChar *goodpic = xmlGetProp(listNode, BAD_CAST "goodpic");    // ��ȡ����ֵ
			xmlChar *coin = xmlGetProp(listNode, BAD_CAST "coin");    // ��ȡ����ֵ
			xmlChar *goodtype = xmlGetProp(listNode, BAD_CAST "goodtype");    // ��ȡ����ֵ
			xmlChar *flag = xmlGetProp(listNode, BAD_CAST "flag");    // ��ȡ����ֵ
			xmlChar *onoff = xmlGetProp(listNode, BAD_CAST "onoff");    // ��ȡ����ֵ
			xmlChar *platcoincount = xmlGetProp(listNode, BAD_CAST "platcoincount");    // ��ȡ����ֵ
			xmlChar *platcointype = xmlGetProp(listNode, BAD_CAST "platcointype");    // ��ȡ����ֵ

			MysShop mysShop;
			XML_SAFE_PARSE_STRING(mysShop.desc, desc, "desc");
			XML_SAFE_PARSE_STRING(mysShop.goodName, goodname, "goodname");
			XML_SAFE_PARSE_INT(mysShop.goodId, goodid, "goodid");
			XML_SAFE_PARSE_FLOAT(mysShop.prePrice, preprice, "preprice");
			XML_SAFE_PARSE_FLOAT(mysShop.price, price, "price");
			XML_SAFE_PARSE_STRING(mysShop.goodpic, goodpic, "goodpic");
			XML_SAFE_PARSE_INT(mysShop.coin, coin, "coin");
			XML_SAFE_PARSE_INT(mysShop.goodtype, goodtype, "goodtype");
			XML_SAFE_PARSE_INT(mysShop.flag, flag, "flag");
			XML_SAFE_PARSE_INT(mysShop.onoff, onoff, "onoff");
			XML_SAFE_PARSE_INT(mysShop.platcoincount, platcoincount, "platcoincount");
			XML_SAFE_PARSE_STRING(mysShop.platcointype, platcointype, "platcointype");

			xmlFree(goodid);
			xmlFree(goodname);
			xmlFree(preprice);
			xmlFree(price);
			xmlFree(desc);
			xmlFree(goodpic);
			xmlFree(coin);
			xmlFree(goodtype);
			xmlFree(flag);
			xmlFree(onoff);
			xmlFree(platcoincount);
			xmlFree(platcointype);

			shopVec.push_back(mysShop);			
		}
		listNode = listNode->next;
	}
	m_mysShops[strChannel] = shopVec;
}

void GameConfig::parseSwitchs(xmlNodePtr node)
{
	xmlNodePtr listNode = NULL;
	listNode = node->xmlChildrenNode;
	while (listNode)
	{
		if (xmlStrcmp(listNode->name, BAD_CAST "switch")==0)
		{
			xmlChar *key = xmlGetProp(listNode, BAD_CAST "key");    // ��ȡ����ֵ
			xmlChar *value = xmlGetProp(listNode, BAD_CAST "value");    // ��ȡ����ֵ
			
			string strKey;
			int nValue;
			XML_SAFE_PARSE_STRING(strKey, key, "switchkey");
			XML_SAFE_PARSE_INT(nValue, value, "switchValue");
			
			xmlFree(key);
			xmlFree(value);

			m_switchs[strKey] = nValue>0 ? true : false;
		}
		listNode = listNode->next;
	}
}
void GameConfig::parseLanguages(xmlNodePtr node)
{
	xmlNodePtr listNode = NULL;
	listNode = node->xmlChildrenNode;
	m_LanguageInfos.clear();
	while (listNode)
	{
		if (xmlStrcmp(listNode->name, BAD_CAST "Language")==0)
		{
			xmlChar *name = xmlGetProp(listNode, BAD_CAST "name");    // ��ȡ����ֵ
			xmlChar *des = xmlGetProp(listNode, BAD_CAST "des");    // ��ȡ����ֵ
			LanguageInfo info;
			
			XML_SAFE_PARSE_STRING(info.name, name, "name");
			XML_SAFE_PARSE_STRING(info.des, des, "des");
			xmlFree(name);
			xmlFree(des);
			m_LanguageInfos.push_back(info);
		}
		listNode = listNode->next;
	}
}
void GameConfig::parseCommonData(xmlNodePtr node)
{
	xmlNodePtr listNode = NULL;
	listNode = node->xmlChildrenNode;
	while (listNode)
	{
		if (xmlStrcmp(listNode->name, BAD_CAST "commondata")==0)
		{
			xmlChar *key = xmlGetProp(listNode, BAD_CAST "key");    // ��ȡ����ֵ
			xmlChar *data = xmlGetProp(listNode, BAD_CAST "data");    // ��ȡ����ֵ
			
			string strKey;
			string nValue;
			XML_SAFE_PARSE_STRING(strKey, key, "key");
			XML_SAFE_PARSE_STRING(nValue, data, "data");
			xmlFree(key);
			xmlFree(data);

			m_commonData[strKey] = nValue;
		}
		listNode = listNode->next;
	}
}
void GameConfig::parseRegions(xmlNodePtr node)
{
	xmlNodePtr listNode = NULL;
	listNode = node->xmlChildrenNode;
	while (listNode)
	{
		if (xmlStrcmp(listNode->name, BAD_CAST "region")==0)
		{
			xmlChar *senceid = xmlGetProp(listNode, BAD_CAST "senceid");    // ��ȡ����ֵ
			xmlChar *normalpic = xmlGetProp(listNode, BAD_CAST "normalpic");    // ��ȡ����ֵ
			xmlChar *touchpic = xmlGetProp(listNode, BAD_CAST "touchpic");    // ��ȡ����ֵ
			xmlChar *spacey = xmlGetProp(listNode, BAD_CAST "spacey");    // ��ȡ����ֵ
			xmlChar *sencetype = xmlGetProp(listNode, BAD_CAST "sencetype");    // ��ȡ����ֵ
			xmlChar *name = xmlGetProp(listNode, BAD_CAST "name");    // ��ȡ����ֵ
			xmlChar *desc = xmlGetProp(listNode, BAD_CAST "desc");    // ��ȡ����ֵ

			RegionInfo regionInfo;
			XML_SAFE_PARSE_STRING(regionInfo.normalpic, normalpic, "normalpic");
			XML_SAFE_PARSE_STRING(regionInfo.touchpic, touchpic, "touchpic");
			XML_SAFE_PARSE_INT(regionInfo.senceid, senceid, "senceid");
			XML_SAFE_PARSE_INT(regionInfo.spacey, spacey, "spacey");
			XML_SAFE_PARSE_INT(regionInfo.sencetype, sencetype, "sencetype");
			XML_SAFE_PARSE_STRING(regionInfo.name, name, "name");
			XML_SAFE_PARSE_STRING(regionInfo.desc, desc, "desc");

			xmlFree(senceid);
			xmlFree(normalpic);
			xmlFree(touchpic);
			xmlFree(spacey);
			xmlFree(sencetype);
			xmlFree(name);
			xmlFree(desc);

			m_regionInfos.push_back(regionInfo);
		}
		listNode = listNode->next;
	}
}

void GameConfig::parseGoods(xmlNodePtr node)
{

	xmlChar *channel = xmlGetProp(node, BAD_CAST "channel");    // ��ȡ����ֵ
	string strChannel;
	XML_SAFE_PARSE_STRING(strChannel, channel, "channel");

	xmlNodePtr listNode = NULL;
	vector<GoodInfo> goods;
	listNode = node->xmlChildrenNode;
	while (listNode)
	{
		if (xmlStrcmp(listNode->name, BAD_CAST "good")==0)
		{
			xmlChar *goodid = xmlGetProp(listNode, BAD_CAST "goodid");    // ��ȡ����ֵ
			xmlChar *goodname = xmlGetProp(listNode, BAD_CAST "goodname");    // ��ȡ����ֵ
			xmlChar *gooddesc = xmlGetProp(listNode, BAD_CAST "gooddesc");    // ��ȡ����ֵ
			xmlChar *goodpic = xmlGetProp(listNode, BAD_CAST "goodpic");    // ��ȡ����ֵ
			xmlChar *goodmoney = xmlGetProp(listNode, BAD_CAST "goodmoney");    // ��ȡ����ֵ
			xmlChar *coin = xmlGetProp(listNode, BAD_CAST "coin");    // ��ȡ����ֵ
			xmlChar *goodtype = xmlGetProp(listNode, BAD_CAST "goodtype");    // ��ȡ����ֵ
			xmlChar *flag = xmlGetProp(listNode, BAD_CAST "flag");    // ��ȡ����ֵ
			xmlChar *onoff = xmlGetProp(listNode, BAD_CAST "onoff");    // ��ȡ����ֵ
			xmlChar *platcoincount = xmlGetProp(listNode, BAD_CAST "platcoincount");    // ��ȡ����ֵ
			xmlChar *platcointype = xmlGetProp(listNode, BAD_CAST "platcointype");    // ��ȡ����ֵ
			xmlChar *extracoins = xmlGetProp(listNode, BAD_CAST "extracoins");    // ��ȡ����ֵ
			xmlChar *itemtype = xmlGetProp(listNode, BAD_CAST "itemtype");    // ��ȡ����ֵ
			xmlChar *goodstype = xmlGetProp(listNode, BAD_CAST "goodstype");    // ��ȡ����ֵ

			GoodInfo good;
			XML_SAFE_PARSE_STRING(good.goodname, goodname, "goodname");
			XML_SAFE_PARSE_STRING(good.gooddesc, gooddesc, "gooddesc");
			XML_SAFE_PARSE_STRING(good.goodpic, goodpic, "goodpic");
			XML_SAFE_PARSE_INT(good.goodid, goodid, "goodid");
			XML_SAFE_PARSE_FLOAT(good.goodmoney, goodmoney, "goodmoney");
			XML_SAFE_PARSE_INT(good.coin, coin, "coin");
			XML_SAFE_PARSE_INT(good.goodtype, goodtype, "goodtype");
			XML_SAFE_PARSE_INT(good.flag, flag, "flag");
			XML_SAFE_PARSE_INT(good.onoff, onoff, "onoff");
			XML_SAFE_PARSE_INT(good.platcoincount, platcoincount, "platcoincount");
			XML_SAFE_PARSE_STRING(good.platcointype, platcointype, "platcointype");
			XML_SAFE_PARSE_INT(good.extracoins, extracoins, "extracoins");
			XML_SAFE_PARSE_INT(good.itemtype, itemtype, "itemtype");
			XML_SAFE_PARSE_INT(good.goodsType, goodstype, "goodstype");

			xmlFree(goodid);
			xmlFree(goodname);
			xmlFree(gooddesc);
			xmlFree(goodpic);
			xmlFree(goodmoney);
			xmlFree(coin);
			xmlFree(goodtype);
			xmlFree(flag);
			xmlFree(onoff);
			xmlFree(platcoincount);
			xmlFree(platcointype);
			xmlFree(extracoins);
			xmlFree(itemtype);
			xmlFree(goodstype);
			goods.push_back(good);
		}
		listNode = listNode->next;
	}

	m_goodInfos[strChannel] = goods;
}

void GameConfig::parseScenes(xmlNodePtr node)
{
	xmlNodePtr listNode = NULL;
	listNode = node->xmlChildrenNode;
	while (listNode)
	{
		if (xmlStrcmp(listNode->name, BAD_CAST "table")==0)
		{
			xmlChar *sencetype = xmlGetProp(listNode, BAD_CAST "scenetype");    // ��ȡ����ֵ
			xmlChar *multyuser = xmlGetProp(listNode, BAD_CAST "multyuser");    // ��ȡ����ֵ
			xmlChar *goldup = xmlGetProp(listNode, BAD_CAST "goldup");    // ��ȡ����ֵ
			xmlChar *goldlower = xmlGetProp(listNode, BAD_CAST "goldlower");    // ��ȡ����ֵ
			xmlChar *pointup = xmlGetProp(listNode, BAD_CAST "pointup");    // ��ȡ����ֵ
			xmlChar *pointlower = xmlGetProp(listNode, BAD_CAST "pointlower");    // ��ȡ����ֵ
			xmlChar *pourcoinup = xmlGetProp(listNode, BAD_CAST "pourcoinup");    // ��ȡ����ֵ
			xmlChar *pourcoinlower = xmlGetProp(listNode, BAD_CAST "pourcoinlower");    // ��ȡ����ֵ
			xmlChar *ticket = xmlGetProp(listNode, BAD_CAST "ticket");    // ��ȡ����ֵ
			xmlChar *desc = xmlGetProp(listNode, BAD_CAST "desc");    // ��ȡ����ֵ
			xmlChar *peopleup = xmlGetProp(listNode, BAD_CAST "peopleup");    // ��ȡ����ֵ
			xmlChar *peoplelower = xmlGetProp(listNode, BAD_CAST "peoplelower");    // ��ȡ����ֵ
			xmlChar *skin = xmlGetProp(listNode, BAD_CAST "skin");    // ��ȡ����ֵ
			xmlChar *flag = xmlGetProp(listNode, BAD_CAST "flag");    // ��ȡ����ֵ
			xmlChar *goodid = xmlGetProp(listNode, BAD_CAST "goodid");    // ��ȡ����ֵ
			xmlChar *goodcount = xmlGetProp(listNode, BAD_CAST "goodcount");    // ��ȡ����ֵ
			xmlChar *vipgoodid = xmlGetProp(listNode, BAD_CAST "vipgoodid");    // ��ȡ����ֵ
			TableConfig tableConfig;
			XML_SAFE_PARSE_STRING(tableConfig.desc, desc, "desc");
			XML_SAFE_PARSE_INT(tableConfig.scenetype, sencetype, "sencetype");
			XML_SAFE_PARSE_INT(tableConfig.multyuser, multyuser, "multyuser");
			XML_SAFE_PARSE_INT(tableConfig.goldup, goldup, "goldup");
			XML_SAFE_PARSE_INT(tableConfig.goldlower, goldlower, "goldlower");
			XML_SAFE_PARSE_INT(tableConfig.piontup, pointup, "pointup");
			XML_SAFE_PARSE_INT(tableConfig.pointlower, pointlower, "pointlower");
			XML_SAFE_PARSE_INT(tableConfig.pourcoinup, pourcoinup, "pourcoinup");
			XML_SAFE_PARSE_INT(tableConfig.pourcoinlower, pourcoinlower, "pourcoinlower");
			XML_SAFE_PARSE_INT(tableConfig.ticket, ticket, "ticket");
			XML_SAFE_PARSE_INT(tableConfig.peopleup, peopleup, "people");
			XML_SAFE_PARSE_INT(tableConfig.peoplelower, peoplelower, "peoplelower");
			XML_SAFE_PARSE_INT(tableConfig.skin, skin, "skin");
			XML_SAFE_PARSE_INT(tableConfig.flag, flag, "flag");
			XML_SAFE_PARSE_INT(tableConfig.goodid, goodid, "goodid");
			XML_SAFE_PARSE_INT(tableConfig.goodcount, goodcount, "goodcount");
			XML_SAFE_PARSE_INT(tableConfig.vipgoodid, vipgoodid, "vipgoodid");
			xmlFree(sencetype);
			xmlFree(multyuser);
			xmlFree(goldup);
			xmlFree(goldlower);
			xmlFree(pointup);
			xmlFree(pointlower);
			xmlFree(pourcoinup);
			xmlFree(pourcoinlower);
			xmlFree(ticket);
			xmlFree(desc);
			xmlFree(peopleup);
			xmlFree(peoplelower);
			xmlFree(skin);
			xmlFree(flag);
			xmlFree(goodid);
			xmlFree(goodcount);
			xmlFree(vipgoodid);
			m_tableConfigs.push_back(tableConfig);
		}
		listNode = listNode->next;
	}
}

void GameConfig::parseLevelInfo(xmlNodePtr node)
{
	xmlNodePtr listNode = NULL;
	listNode = node->xmlChildrenNode;
	while (listNode)
	{
		if (xmlStrcmp(listNode->name, BAD_CAST "level")==0)
		{
			xmlChar *point = xmlGetProp(listNode, BAD_CAST "point");    // ��ȡ����ֵ
			xmlChar *level = xmlGetProp(listNode, BAD_CAST "level");    // ��ȡ����ֵ
			xmlChar *levelname = xmlGetProp(listNode, BAD_CAST "levelname");    // ��ȡ����ֵ

			LevelInfo levelInfo;
			XML_SAFE_PARSE_STRING(levelInfo.level, level, "level");
			XML_SAFE_PARSE_STRING(levelInfo.levelname, levelname, "levelname");
			XML_SAFE_PARSE_INT(levelInfo.point, point, "point");
			
			xmlFree(point);
			xmlFree(level);
			xmlFree(levelname);

			m_levelInfos.push_back(levelInfo);
		}
		listNode = listNode->next;
	}
}

xmlNodePtr GameConfig::getNode(string noteName)
{
	xmlDocPtr   pdoc = NULL;
	xmlNodePtr  proot = NULL;
	xmlNodePtr  curNode = NULL;

	pdoc = xmlParseMemory ((char*)m_gameConfig.c_str(), m_gameConfig.length());

	if (pdoc == NULL)
	{
		return NULL;
	}

	// ��ȡ xml �ĵ�����ĸ��ڶ���
	proot = xmlDocGetRootElement (pdoc);

	if (proot == NULL)
	{
		return NULL;
	}

	curNode = proot->xmlChildrenNode;
	if (!curNode)
	{
		return NULL;
	}

	while (curNode != NULL)
	{
		if (xmlStrcmp(curNode->name, BAD_CAST noteName.c_str())==0)
		{
			return curNode;
		}
		curNode = curNode->next;
	}

	/* �رպ����� */
	xmlFreeDoc(pdoc);
	xmlCleanupParser();

	return NULL;
}

MysShop GameConfig::getMysShop(int goodid, string channel)
{
	MysShop mysShop;
	mysShop.goodId = -1;

	vector<MysShop> myShops = getMysShop(channel);
	vector<MysShop>::iterator it = myShops.begin();
	while (it != myShops.end())
	{
		if (it->goodId == goodid)
		{
			mysShop = *it;
			break;
		}
		it++;
	}
	return mysShop;


}

bool GameConfig::isSwitchOpen(string switchName)
{
	map<string, bool>::iterator it = m_switchs.find(switchName);
	if (it!=m_switchs.end())
	{
		return it->second;
	}
	return false;
}
string GameConfig::getCommondata(string commondata)
{
	map<string, string>::iterator it = m_commonData.find(commondata);
	if (it!=m_commonData.end())
	{
		return it->second;
	}
	return "";
}


void GameConfig::parseDebug(xmlNodePtr node)
{
	xmlChar *isDebug = xmlGetProp(node, BAD_CAST "switch");    // ��ȡ����ֵ
	xmlChar *ip = xmlGetProp(node, BAD_CAST "ip");    // ��ȡ����ֵ
	xmlChar *port = xmlGetProp(node, BAD_CAST "port");    // ��ȡ����ֵ

	string debug;
	XML_SAFE_PARSE_STRING(m_debug.ip, ip, "ip");
	XML_SAFE_PARSE_STRING(debug, isDebug, "isdebug");
	XML_SAFE_PARSE_INT(m_debug.port, port, "port");

	m_debug.isDebug = debug=="true" ? true : false;

	xmlFree(isDebug);
	xmlFree(ip);
	xmlFree(port);
}

vector<TableConfig> GameConfig::getTableConfigs()
{
	return m_tableConfigs;
}

vector<GoodInfo> GameConfig::getGoodsInfo(string channel)
{
	//��ȡ������
	int index = channel.find_first_of('.');
	if (index > 0)
	{
		channel = channel.substr(0, index);
	}

	if (m_goodInfos[channel].size() > 0)
	{
		return m_goodInfos[channel];
	}
	return m_goodInfos["default"];
}

vector<RegionInfo> GameConfig::getRegionInfos()
{
	return m_regionInfos;
}
vector<LanguageInfo> GameConfig::getLanguageInfos()
{
	return m_LanguageInfos;
}
vector<LevelInfo> GameConfig::getLevelInfos()
{
	return m_levelInfos;
}

vector<MysShop> GameConfig::getMysShop(string channel)
{
	//��ȡ������
	int index = channel.find_first_of('.');
	if (index > 0)
	{
		channel = channel.substr(0, index);
	}

	if (m_mysShops[channel].size() > 0)
	{
		return m_mysShops[channel];
	}
	return m_mysShops["default"];
}

TableConfig* GameConfig::getTableConfig(int scenetype)
{
	for (int i = 0; i < m_tableConfigs.size(); i++)
	{
		if (m_tableConfigs[i].scenetype == scenetype)
		{
			return &m_tableConfigs[i];
		}
	}
	return NULL;
}

bool GameConfig::getGoodInfo(int goodid, GoodInfo &goodInfo, string channel)
{
	//��ȡ������
	int index = channel.find_first_of('.');
	if (index > 0)
	{
		channel = channel.substr(0, index);
	}

	vector<GoodInfo> goodsInfo = getGoodsInfo(channel);

	for (int i = 0; i < goodsInfo.size(); i++)
	{
		if (goodsInfo[i].goodid == goodid)
		{
			goodInfo = goodsInfo[i];
			return true;
		}
	}
	return false;
}

RegionInfo* GameConfig::getRegionInfo(int senceid)
{
	for (int i = 0; i < m_regionInfos.size(); i++)
	{
		if (m_regionInfos[i].senceid == senceid)
		{
			return &m_regionInfos[i];
		}
	}
	return NULL;
}

