#include "ZSound.h"
#include "utils/GameSoundEff.h"
#include "utils/GameResDown.h"
#include "utils/GameConfig.h"
#include "cocos2d.h"
#include "Lobby.h"

USING_NS_CC;

CZSound* CZSound::m_pInstance = NULL;

CZSound::CZSound(void)
{
	
}


CZSound::~CZSound(void)
{
	
}

CZSound*CZSound::Instance()
{
	if (!m_pInstance)
	{
		m_pInstance=new CZSound();
	}
	return m_pInstance;
}

void CZSound::initGameSound()//初始化音效
{
	//动态下载音效

	string savePath =GameResDown::instance()->getResPath(g_pGameConfig->getGameID());
	map<int, string> &map_saveSound = g_lobby.getSound();

	map_saveSound[other_btn]= /*savePath+*/"sound/Others/btn.mp3";//内置

	map_saveSound[kEffectDeshion0_follow]= savePath+"sound/OX_CALL_FEMALE_SOUND.ogg";
	map_saveSound[kEffectDeshion0_add]= savePath+"sound/OX_RAISE_FEMALE_SOUND.ogg";
	map_saveSound[kEffectDeshion0_discard]= savePath+"sound/OX_FOLD_FEMALE_SOUND.ogg";
	map_saveSound[kEffectDeshion0_allin]= savePath+"sound/OX_ALLIN_FEMALE_SOUND.ogg";

	map_saveSound[kEffectDeshion0_wait]= savePath+"sound/wait.mp3";//
	map_saveSound[kEffectDeshion0_qz]= savePath+"sound/qz.mp3";
	map_saveSound[kEffectDeshion0_xz]= savePath+"sound/xz.mp3";

	map_saveSound[kEffectDeshion0_mn]= savePath+"sound/mn.ogg" ;
	map_saveSound[kEffectDeshion0_n1]= savePath+"sound/n1.ogg" ;
	map_saveSound[kEffectDeshion0_n2]= savePath+"sound/n2.ogg" ;
	map_saveSound[kEffectDeshion0_n3]= savePath+"sound/n3.ogg" ;
	map_saveSound[kEffectDeshion0_n4]= savePath+"sound/n4.ogg" ;
	map_saveSound[kEffectDeshion0_n5]= savePath+"sound/n5.ogg" ;
	map_saveSound[kEffectDeshion0_n6]= savePath+"sound/n6.ogg" ;
	map_saveSound[kEffectDeshion0_n7]= savePath+"sound/n7.ogg" ;
	map_saveSound[kEffectDeshion0_n8]= savePath+"sound/n8.ogg" ;
	map_saveSound[kEffectDeshion0_n9]= savePath+"sound/n9.ogg" ;
	map_saveSound[kEffectDeshion0_nn]= savePath+"sound/nn.ogg" ;
	map_saveSound[kEffectDeshion0_nb]= savePath+"sound/nb.ogg" ;
	map_saveSound[kEffectDeshion0_nx]= savePath+"sound/nx.ogg" ;

	map_saveSound[others_win]= savePath+"sound/win.mp3" ;//old win result
	map_saveSound[others_send]= savePath+"sound/send.mp3" ;//need change
	map_saveSound[others_shock]= savePath+"sound/comparepoker_shadian.mp3" ;
	map_saveSound[others_finish_task]= savePath+"sound/finish_task.mp3" ;
	map_saveSound[others_bet]= savePath+"sound/bet.mp3" ;
	map_saveSound[others_compair_win]= savePath+"sound/compair_win.mp3" ;
	map_saveSound[others_compair_fail]= savePath+"sound/compair_fail.mp3" ;
	map_saveSound[others_clap]= savePath+"sound/clap.mp3" ;//winer  a game 
	map_saveSound[ESoundEffectInRoom]= savePath+"sound/door.mp3" ;
	map_saveSound[ESoundEffectOutmenu]= savePath+"sound/outingMenu.mp3" ;

	map_saveSound[others_5s]= savePath+"sound/tick.mp3" ;
	//yaoyao le
	map_saveSound[ESoundEffectTigerPlay]= savePath+"sound/tigerplay.mp3" ;
	map_saveSound[ESoundEffectTigerPocker]= savePath+"sound/tigerpocker.mp3" ;
	map_saveSound[ESoundEffectTigerWin]= savePath+"sound/tiger_win.mp3" ;

	//Dynamic
	map_saveSound[ESoundEffectZhaSiNi]= savePath+"sound/zhasini.mp3";
	map_saveSound[ESoundEffectDianSiNi]= savePath+"sound/diansini.mp3";
	map_saveSound[ESoundEffectZaSiNi]= savePath+"sound/zasini.mp3";
	map_saveSound[ESoundEffectZanYiXia]= savePath+"sound/zanyixia.mp3";
	map_saveSound[ESoundEffectNiWeiWu]= savePath+"sound/niweiwu.mp3";

}

unsigned int CZSound::playSound(int soundID,bool bloop)
{
	if (other_btn == soundID )
	{
		return GameSoundEff::Instance()->playSoundEff(soundID,bloop);
	}
	map<int, string> map_saveSound = g_lobby.getSound();
	map<int, string>::iterator it;
	it = map_saveSound.find(soundID);
	if (it !=map_saveSound.end())
	{
		return 0;
	}
	//CCLog("plat sound:%d",soundID);
	return GameSoundEff::Instance()->playSoundEff(soundID,bloop);
}

void CZSound::stopSound(int soundID)
{
	CCLog("play sound :%d", soundID);
	GameSoundEff::Instance()->stopSoundEff(soundID);
}

void CZSound::stopAllSound()
{
	GameSoundEff::Instance()->stopAllSoundEff();
}

void CZSound::initButtonSound()
{
	string savePath =GameResDown::instance()->getResPath(g_pGameConfig->getGameID());
	map<int, string> &map_saveSound = g_lobby.getSound();
	map_saveSound[other_btn]= /*savePath+*/"sound/Others/btn.mp3";//内置
}
