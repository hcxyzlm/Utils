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
	int scenetype;			//桌子类型	
	int multyuser;			//多人
	int goldup;				//进入Gold上限	
	int goldlower;			//进入Gold下限	
	int piontup;			//进入积分上限	
	int pointlower;			//进入积分下限	
	int pourcoinup;			//下注coin上线
	int pourcoinlower;		//下注coin下线	
	int ticket;				//门票	
	string desc;			//描述	
	int peopleup;			//人数上限	
	int peoplelower;		//人数下限
	int skin;				//地皮
	int flag;               //特殊标识（火爆，双倍场等）
	int goodid;				//快捷购买商品id
	int goodcount;          //快捷购买商品个数
	int vipgoodid;          //vip快捷购买商品id
};

struct GoodInfo
{
	int goodid;
	string goodname; 
	string gooddesc; 
	string goodpic;
	float goodmoney;
	int coin;
	int goodtype;      //商品类型：金币，道具...
	int flag;          //打折等标识
	int onoff;      //开关标识 0关闭，1开启
	int platcoincount;   //第三方获取数量
	string platcointype;  //第三方货币单位
	int extracoins;   //商城额外赠送
	int itemtype;//道具类型 联网 单机...
	int goodsType;//平台币
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

//破产保护
struct ProtectGiveConf
{
	int lowcoin;
	int givecoin;
	int count;
};

//连续登录奖励
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

//对局任务奖励
struct PlayGiveConf
{
	int scenceid;
	vector<SencePlayGive> sencePlayGives;
};

//在线礼包奖励
struct TimeGivecConf
{
	int time;
	int coin;
	int fare;
};

//vip礼包
struct VipInfo
{
	int level;      //vip等级
	int topRechage;    //累计充值 
	int giveCoin;      //赠送游戏币 
	int goodId;        //对应神秘商店礼包
};

//神秘商店
struct MysShop
{
	int goodId;        //商品id
	string goodName;   //商品名
	float prePrice;      //原价
	float price;         //现价 
	string desc;       //描述
	string goodpic;
	int coin;
	int goodtype;      //商品类型：金币，道具...
	int flag;          //打折等标识
	int onoff;      //开关标识 0关闭，1开启
	int platcoincount;   //第三方获取数量
	string platcointype;  //第三方货币单位
	int extracoins;   //商城额外赠送
	
};

class GameConfig
{
public:
	GameConfig();
	~GameConfig();

	int		getGameID();    //获得游戏ID
	int		getConfigVersion(){return m_configVersion;}  //获取资源版本号
	ProtectGiveConf getProptectInfo();  //获取破产保护下线和赠送金币数
	string	getPlatLoginUrl();   //获取平台登录验证url
	string	getServerListUtl();  //获取serverlist url
	string	getVerUpdateUrl();   //获取版本升级验证url
	string	getModifyUserInfoUrl();  //获取用户信息修改url
	string  getGameConfigUrl();  //获取获得游戏配置信息的url
	string  getExchangeUrl();  //获取话费劵兑换url
	string  getNoticeUrl();  //获取公告兑换url
	string  getMoreGameUrl();  //获取更多游戏url
	string  getRecommondUrl();  //获取推荐serverid接口
	
	string  getWebUrl(string urlKey);  //获取URL
	
	string  getGameXmlInfo();  //获取整个xml配置文件

	bool parseGameConfig(char *pBuffer, unsigned long nSize);  //解析数据流
	
	vector<TableConfig> getTableConfigs();
	vector<GoodInfo> getGoodsInfo(string channel="default");
	vector<RegionInfo> getRegionInfos();
	vector<LanguageInfo> getLanguageInfos();
	vector<LevelInfo> getLevelInfos();
	vector<MysShop> getMysShop(string channel="default");   //神秘商店

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

	vector<LoginGiveConf> getLoginGive(){return m_loginGive;} //连续登录奖励
	vector<PlayGiveConf> getPlayGive(){return m_playGive;}   //对局任务奖励
	vector<TimeGivecConf> getTimeGive(){return m_timeGive;}   //在线礼包奖励
	bool isSwitchOpen(string switchName);    //查询开关是否打开
	string getCommondata(string commondata);//查询数据
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

	vector<LoginGiveConf> m_loginGive; //连续登录奖励
	vector<PlayGiveConf> m_playGive;   //对局任务奖励
	vector<TimeGivecConf> m_timeGive;   //在线礼包奖励
	vector<GoodInfo>	m_goodInfoByChannel;
	map<string, bool> m_switchs;   //各种开关
	map<string, string> m_commonData;   //配置
};

extern GameConfig *g_pGameConfig;

#endif