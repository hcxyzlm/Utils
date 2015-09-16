#include "GameInfoSave.h"
#include "utils/Util.h"
#include "GameSDK.h"

#include "cocos2d.h"
USING_NS_CC;

GameInfoSave* GameInfoSave::pIntance = NULL;

GameInfoSave::GameInfoSave()
{

}

GameInfoSave::~GameInfoSave()
{

}

GameInfoSave* GameInfoSave::instance()
{
	if (!pIntance)
	{
		pIntance = new GameInfoSave();
	}
	return pIntance;
}

void GameInfoSave::setNickName(int userid, string value)
{
	string str = "nick";
	str.append(int2string(userid).c_str());
	setStringForKey(str.c_str(), value);
}

string GameInfoSave::getNickName(int userid)
{
	string str = "nick";
	str.append(int2string(userid).c_str());
	return getStringForKey(str.c_str());
}

void GameInfoSave::setMan(int userid, bool isMan)
{
	string str = "gender";
	str.append(int2string(userid).c_str());
	setBoolForKey(str.c_str(), isMan);
}

bool GameInfoSave::isMan(int userid)
{
	string str = "gender";
	str.append(int2string(userid).c_str());
	return getBoolForKey(str.c_str(), true);
}

void GameInfoSave::setGameShock(bool shock)
{
	CCUserDefault::sharedUserDefault()->setBoolForKey("game_shock", shock);
	CCUserDefault::sharedUserDefault()->flush();
}

bool GameInfoSave::getGameShock()
{
	return CCUserDefault::sharedUserDefault()->getBoolForKey("game_shock", true);
}

void GameInfoSave::setGameElec(bool elec)
{
	CCUserDefault::sharedUserDefault()->setBoolForKey("game_elec", elec);
	CCUserDefault::sharedUserDefault()->flush();
}

bool GameInfoSave::getGameElec()
{
	return CCUserDefault::sharedUserDefault()->getBoolForKey("game_elec", false);
}

void GameInfoSave::setVoice(bool isVoice)
{
    CCUserDefault::sharedUserDefault()->setBoolForKey("game_voice", isVoice);
    CCUserDefault::sharedUserDefault()->flush();
}

bool GameInfoSave::getVoice()
{
    return CCUserDefault::sharedUserDefault()->getBoolForKey("game_voice", true);
}
void GameInfoSave::setHeadurl(int userid, string headurl)
{
	string str = "Headurl";
	str.append(int2string(userid).c_str());
	CCUserDefault::sharedUserDefault()->setStringForKey(str.c_str(), headurl);
	CCUserDefault::sharedUserDefault()->flush();
}

string GameInfoSave::getHeadurl(int userid)
{
	string str = "Headurl";
	str.append(int2string(userid).c_str());
	return CCUserDefault::sharedUserDefault()->getStringForKey(str.c_str(), "");
}
void GameInfoSave::setHeadIndex(int userid, int index)
{
	string str = "headindex";
	str.append(int2string(userid).c_str());
	CCUserDefault::sharedUserDefault()->setIntegerForKey(str.c_str(), index);
	CCUserDefault::sharedUserDefault()->flush();
}

int GameInfoSave::getHeadIndex(int userid)
{
	string str = "headindex";
	str.append(int2string(userid).c_str());
	return CCUserDefault::sharedUserDefault()->getIntegerForKey(str.c_str(), 0);
}

void GameInfoSave::setPlatMusic(bool bValue)
{
	CCLog("set plat music :%d", bValue ? 1 : 0);
	CCUserDefault::sharedUserDefault()->setBoolForKey("plat_music", bValue);
    CCUserDefault::sharedUserDefault()->flush();
}

bool GameInfoSave::getPlatMusic()
{
	bool ret = CCUserDefault::sharedUserDefault()->getBoolForKey("plat_music", true);
	CCLog("get plat music :%d", ret ? 1 : 0);
	return ret;
}

void GameInfoSave::setIntegerForKey(const char* key, int value)
{
	CCUserDefault::sharedUserDefault()->setIntegerForKey(key, value);
	CCUserDefault::sharedUserDefault()->flush();
}

int  GameInfoSave::getIntegerForKey(const char* key, int defaultValue)
{
	return CCUserDefault::sharedUserDefault()->getIntegerForKey(key, defaultValue);
}

void GameInfoSave::setBoolForKey(const char* key, bool value)
{
	CCUserDefault::sharedUserDefault()->setBoolForKey(key, value);
	CCUserDefault::sharedUserDefault()->flush();
}

bool GameInfoSave::getBoolForKey(const char* key, bool defaultValue)
{
	return CCUserDefault::sharedUserDefault()->getBoolForKey(key, defaultValue);
}

void GameInfoSave::setStringForKey(const char* key, string value)
{
	CCUserDefault::sharedUserDefault()->setStringForKey(key, value);
	CCUserDefault::sharedUserDefault()->flush();
}

string GameInfoSave::getStringForKey(const char* key, string defaultValue)
{
	return CCUserDefault::sharedUserDefault()->getStringForKey(key, defaultValue);
}

void GameInfoSave::setFloatForKey(const char* pKey, float value)
{
	CCUserDefault::sharedUserDefault()->setFloatForKey(pKey, value);
	CCUserDefault::sharedUserDefault()->flush();
}

float GameInfoSave::getFloatForKey(const char* pKey, float defaultValue)
{
	return CCUserDefault::sharedUserDefault()->getFloatForKey(pKey, defaultValue);
}

void GameInfoSave::setDoubleForKey(const char* pKey, double value)
{
	CCUserDefault::sharedUserDefault()->setDoubleForKey(pKey, value);
	CCUserDefault::sharedUserDefault()->flush();
}

double GameInfoSave::getDoubleForKey(const char* pKey, double defaultValue)
{
	return CCUserDefault::sharedUserDefault()->getDoubleForKey(pKey, defaultValue);
}
