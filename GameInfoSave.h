#pragma once

#include <iostream>
using namespace std;

enum LoginType
{
	kLoginType91 = 1,   //91帐号登录
	kLoginAccount = 2,    //帐号登录
	kLoginRandom = 3,    //随机帐号登录
};

class GameInfoSave
{
private:
	GameInfoSave();
	~GameInfoSave();

public:
	static GameInfoSave* instance();

	void setNickName(int userid, string value);
	string getNickName(int userid);

	void setMan(int userid, bool isMan);
	bool isMan(int userid);

	void setGameShock(bool shock);
	bool getGameShock();

	void setGameElec(bool elec);  //省电模式
	bool getGameElec();
    
    void setVoice(bool isVoice);
    bool getVoice();

	void setHeadurl(int userid, string headurl);
	string  getHeadurl(int userid);

	void setHeadIndex(int userid, int index);
	int getHeadIndex(int userid);

	//平台音效开启，某些乱七八糟的平台的变态需求，游戏里面慎用
	void setPlatMusic(bool bValue);
	bool getPlatMusic();

	void setIntegerForKey(const char* key, int value);
	int  getIntegerForKey(const char* key, int defaultValue=0);
	void setBoolForKey(const char* key, bool value);
	bool getBoolForKey(const char* key, bool defaultValue=false);
	void setStringForKey(const char* key, string value);
	string getStringForKey(const char* key, string defaultValue="");
	void setFloatForKey(const char* pKey, float value);
	float getFloatForKey(const char* pKey, float defaultValue=0.0f);
    void setDoubleForKey(const char* pKey, double value);
	double getDoubleForKey(const char* pKey, double defaultValue=0.0);
private:
	static GameInfoSave *pIntance;
};