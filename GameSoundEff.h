#ifndef __GameSoundEff_h__
#define __GameSoundEff_h__


#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

#include <map>
#include <iostream>
using namespace std;


class GameSoundEff
{
	GameSoundEff();
public:
	~GameSoundEff();

public:
	static GameSoundEff *Instance();

public:
	void setSoundEff(int index, string soundFile);  //音效    游戏里面的index>100   大厅里面index<=100
	
	void playBgMusic(string musicPath, bool isLoop);
	unsigned int playSoundEff(int index, bool isLoop);
	unsigned int playSoundEff(int index);
	void parseBgMusic();
	void resumeBgMusic();
	void stopBgMusic(bool isRelease);
	void stopSoundEff(int index);
	void stopAllSoundEff();
	
	bool isPlayingBgMusic();

	float getBackgroundMusicVolume();
    void setBackgroundMusicVolume(float volume);

    float getEffectsVolume();
    void setEffectsVolume(float volume);

	void setPlayBgMusic(bool canPlay);
	void setPlaySound(bool canPlay);
private:
	void loadSound(string file);  //加载音效+音乐
	
private:
	bool m_bCanPlayBgMusic;  //是否播放背景音乐
	bool m_bCanPlayEffect;   //是否播放音效
	string m_strBgMusicFile;  
	
	SimpleAudioEngine	*m_pAudioEngine;
	map<int, string> m_soundEffects;
};


#endif
