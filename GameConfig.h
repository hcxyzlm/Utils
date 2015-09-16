#ifndef __GameConfig_h__
#define __GameConfig_h__

#include "../../Core/GameBaseInfo.h"

#include <libxml/parser.h>
#include <libxml/tree.h>

#include <vector>
#include <string>
#include <map>
#include <iostream>
using namespace std;

struct TableConfig
{
	int scenetype;			//��������	
	int multyuser;			//����
	int goldup;				//����Gold����	
	int goldlower;			//����Gold����	
	int piontup;			//�����������	
	int pointlower;			//�����������	
	int pourcoinup;			//��עcoin����
	int pourcoinlower;		//��עcoin����	
	int ticket;				//��Ʊ	
	string desc;			//����	
	int peopleup;			//��������	
	int peoplelower;		//��������
	int skin;				//��Ƥ
	int flag;               //�����ʶ���𱬣�˫�����ȣ�
	int goodid;				//��ݹ�����Ʒid
	int goodcount;          //��ݹ�����Ʒ����
	int vipgoodid;          //vip��ݹ�����Ʒid
};

struct GoodInfo
{
	int goodid;
	string goodname; 
	string gooddesc; 
	string goodpic;
	float goodmoney;
	int coin;
	int goodtype;      //��Ʒ���ͣ���ң�����...
	int flag;          //���۵ȱ�ʶ
	int onoff;      //���ر�ʶ 0�رգ�1����
	int platcoincount;   //��������ȡ����
	string platcointype;  //���������ҵ�λ
	int extracoins;   //�̳Ƕ�������
	int itemtype;//�������� ���� ����...
	int goodsType;//ƽ̨��
};
struct LanguageInfo
{
	string name;
	string des;
};
struct RegionInfo
{
	int senceid;
	string normalpic;
	string touchpic;
	float spacey;
	int	sencetype;
	string name;
	string desc;
};

struct LevelInfo
{
	int point;
	string level;
	string levelname;
};

struct DebugStruct 
{
	bool isDebug;
	string ip;
	int port;
};

//�Ʋ�����
struct ProtectGiveConf
{
	int lowcoin;
	int givecoin;
	int count;
};

//������¼����
struct LoginGiveConf
{
	int day;
	int coin;
	int fare;
};

struct SencePlayGive
{
	int playcount;
	int coin;
	int fare;
};

//�Ծ�������
struct PlayGiveConf
{
	int scenceid;
	vector<SencePlayGive> sencePlayGives;
};

//�����������
struct TimeGivecConf
{
	int time;
	int coin;
	int fare;
};

//vip���
struct VipInfo
{
	int level;      //vip�ȼ�
	int topRechage;    //�ۼƳ�ֵ 
	int giveCoin;      //������Ϸ�� 
	int goodId;        //��Ӧ�����̵����
};

//�����̵�
struct MysShop
{
	int goodId;        //��Ʒid
	string goodName;   //��Ʒ��
	float prePrice;      //ԭ��
	float price;         //�ּ� 
	string desc;       //����
	string goodpic;
	int coin;
	int goodtype;      //��Ʒ���ͣ���ң�����...
	int flag;          //���۵ȱ�ʶ
	int onoff;      //���ر�ʶ 0�رգ�1����
	int platcoincount;   //��������ȡ����
	string platcointype;  //���������ҵ�λ
	int extracoins;   //�̳Ƕ�������
	
};

class GameConfig
{
public:
	GameConfig();
	~GameConfig();

	int		getGameID();    //�����ϷID
	int		getConfigVersion(){return m_configVersion;}  //��ȡ��Դ�汾��
	ProtectGiveConf getProptectInfo();  //��ȡ�Ʋ��������ߺ����ͽ����
	string	getPlatLoginUrl();   //��ȡƽ̨��¼��֤url
	string	getServerListUtl();  //��ȡserverlist url
	string	getVerUpdateUrl();   //��ȡ�汾������֤url
	string	getModifyUserInfoUrl();  //��ȡ�û���Ϣ�޸�url
	string  getGameConfigUrl();  //��ȡ�����Ϸ������Ϣ��url
	string  getExchangeUrl();  //��ȡ���ф��һ�url
	string  getNoticeUrl();  //��ȡ����һ�url
	string  getMoreGameUrl();  //��ȡ������Ϸurl
	string  getRecommondUrl();  //��ȡ�Ƽ�serverid�ӿ�
	
	string  getWebUrl(string urlKey);  //��ȡURL
	
	string  getGameXmlInfo();  //��ȡ����xml�����ļ�

	bool parseGameConfig(char *pBuffer, unsigned long nSize);  //����������
	
	vector<TableConfig> getTableConfigs();
	vector<GoodInfo> getGoodsInfo(string channel="default");
	vector<RegionInfo> getRegionInfos();
	vector<LanguageInfo> getLanguageInfos();
	vector<LevelInfo> getLevelInfos();
	vector<MysShop> getMysShop(string channel="default");   //�����̵�

	TableConfig* getTableConfig(int scenetype);
	bool getGoodInfo(int goodid, GoodInfo &goodInfo, string channel="default");
	RegionInfo* getRegionInfo(int senceid);
	LevelInfo getLevel(int point);
	DebugStruct getDebugInfo(){return m_debug;}
	xmlNodePtr getNode(string noteName);

	ServerShardVec getServerList(){return m_servers;}
	int getResVersion(string module){return m_resVersion[module];}
	VipInfo getVipInfo(int vipLevel){return m_vipInfos[vipLevel];}
	MysShop getMysShop(int goodid, string channel="default");

	vector<LoginGiveConf> getLoginGive(){return m_loginGive;} //������¼����
	vector<PlayGiveConf> getPlayGive(){return m_playGive;}   //�Ծ�������
	vector<TimeGivecConf> getTimeGive(){return m_timeGive;}   //�����������
	bool isSwitchOpen(string switchName);    //��ѯ�����Ƿ��
	string getCommondata(string commondata);//��ѯ����
private:
	void parseConfig();

	void parseGameId(xmlNodePtr node);
	void parseWebs(xmlNodePtr node);
	void parseRegions(xmlNodePtr node);
	void parseGoods(xmlNodePtr node);
	void parseScenes(xmlNodePtr node);
	void parseLevelInfo(xmlNodePtr node);
	void parseDebug(xmlNodePtr node);
	void parseServerList(xmlNodePtr node);
	void parseGaveCoin(xmlNodePtr node);
	void parseResVersion(xmlNodePtr node);
	void parseVip(xmlNodePtr node);
	void parseMysShop(xmlNodePtr node);
	void parseSwitchs(xmlNodePtr node);
	void parseLanguages(xmlNodePtr node);
	void parseCommonData(xmlNodePtr node);
	void parseProtect(xmlNodePtr node);
	void parseLoginGive(xmlNodePtr node);
	void parsePlayGive(xmlNodePtr node);
	void parseTimeGive(xmlNodePtr node);
	vector<SencePlayGive> parseSencePlayGive(xmlNodePtr node);

	int		m_gameID;
	int		m_configVersion;

	vector<TableConfig> m_tableConfigs;
	map<string, vector<GoodInfo> > m_goodInfos;
	vector<RegionInfo>	m_regionInfos;
	vector<LevelInfo>   m_levelInfos;
	ServerShardVec	m_servers;
	ProtectGiveConf m_protectGive;
	vector<LanguageInfo>	m_LanguageInfos;
	map<string, string> m_webUrls;
	string m_gameConfig;

	DebugStruct m_debug;
	map<string, int>m_resVersion;
	map<int, VipInfo> m_vipInfos;
	map<string, vector<MysShop> > m_mysShops;

	vector<LoginGiveConf> m_loginGive; //������¼����
	vector<PlayGiveConf> m_playGive;   //�Ծ�������
	vector<TimeGivecConf> m_timeGive;   //�����������
	vector<GoodInfo>	m_goodInfoByChannel;
	map<string, bool> m_switchs;   //���ֿ���
	map<string, string> m_commonData;   //����
};

extern GameConfig *g_pGameConfig;

#endif