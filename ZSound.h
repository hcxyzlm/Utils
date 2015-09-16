#pragma once

#include <map>
#include <iostream>
using namespace std;

enum SounID
{
	other_btn,//按键
	ESoundEffectInRoom, //进出房间  baselayer soundWithInAndOutRoom用到
	ESoundEffectOutmenu, //弹出菜单

	kEffectDeshion0_mn,
	kEffectDeshion0_n1,
	kEffectDeshion0_n2,
	kEffectDeshion0_n3,
	kEffectDeshion0_n4,
	kEffectDeshion0_n5,
	kEffectDeshion0_n6,
	kEffectDeshion0_n7,
	kEffectDeshion0_n8,
	kEffectDeshion0_n9,
	kEffectDeshion0_nn,
	kEffectDeshion0_nw,
	kEffectDeshion0_nb,
	kEffectDeshion0_nx,
	kEffectDeshion0_qz,
	kEffectDeshion0_xz,
	kEffectDeshion0_wait,
	kEffectDeshion0_follow,
	kEffectDeshion0_add,
	kEffectDeshion0_discard,
	kEffectDeshion0_allin,

	others_win,//胜利
	others_send,//发牌
	others_5s,//倒计时5s
	others_shock,//比牌的电流音
	others_finish_task,//完成任务
	others_bet,//下注
	others_compair_win,//比赢了
	others_compair_fail,//比输了
	others_clap,//百 人结算win
	ESoundEffectTigerPlay,
	ESoundEffectTigerPocker,
	ESoundEffectTigerWin,
	ESoundEffectZhaSiNi, // 炸死你
	ESoundEffectDianSiNi,	// 電死你
	ESoundEffectZaSiNi,	// 砸死你
	ESoundEffectBiLeNi, // 斃了你
	ESoundEffectZanYiXia, // 贊一下
	ESoundEffectNiWeiWu, // 你威武
	ESoundEffectPaoXiuQiu, // 抛绣球
};

class CZSound
{
public:
	CZSound(void);
	virtual ~CZSound(void);
public:
	static CZSound*Instance();
	void initGameSound();
	void initButtonSound();
	unsigned int playSound(int soundID,bool bloop = false);
	void stopSound(int soundID);
	void stopAllSound();
private:
	static CZSound *m_pInstance;
};

