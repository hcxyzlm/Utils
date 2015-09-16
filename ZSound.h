#pragma once

#include <map>
#include <iostream>
using namespace std;

enum SounID
{
	other_btn,//����
	ESoundEffectInRoom, //��������  baselayer soundWithInAndOutRoom�õ�
	ESoundEffectOutmenu, //�����˵�

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

	others_win,//ʤ��
	others_send,//����
	others_5s,//����ʱ5s
	others_shock,//���Ƶĵ�����
	others_finish_task,//�������
	others_bet,//��ע
	others_compair_win,//��Ӯ��
	others_compair_fail,//������
	others_clap,//�� �˽���win
	ESoundEffectTigerPlay,
	ESoundEffectTigerPocker,
	ESoundEffectTigerWin,
	ESoundEffectZhaSiNi, // ը����
	ESoundEffectDianSiNi,	// �����
	ESoundEffectZaSiNi,	// ������
	ESoundEffectBiLeNi, // ������
	ESoundEffectZanYiXia, // ٝһ��
	ESoundEffectNiWeiWu, // ������
	ESoundEffectPaoXiuQiu, // ������
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

