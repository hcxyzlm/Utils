#include "GameSoundEff.h"
#include "GameInfoSave.h"
#include "cocos2d.h"
USING_NS_CC;

GameSoundEff::GameSoundEff()
{
	m_bCanPlayBgMusic = true;
	m_bCanPlayEffect = true;
	m_pAudioEngine = new SimpleAudioEngine();
}

GameSoundEff::~GameSoundEff()
{
	if (m_pAudioEngine)
	{
		m_pAudioEngine->stopAllEffects();
		m_pAudioEngine->stopBackgroundMusic(true);
		delete m_pAudioEngine;
	}
}

GameSoundEff *GameSoundEff::Instance()
 {
	 static GameSoundEff *pInstance = NULL;
	 if (pInstance == NULL)
	 {
		 pInstance = new GameSoundEff();
	 }

	 return pInstance;

 }


void GameSoundEff::setSoundEff(int index, string soundFile)  //音效
{
	m_soundEffects[index] = soundFile;
	loadSound(soundFile);
}

void GameSoundEff::playBgMusic(string musicPath, bool isLoop)
{
	CCLog("play bg music");
	if (!GameInfoSave::instance()->getPlatMusic())
	{
		return;
	}
	if (m_pAudioEngine && m_bCanPlayBgMusic)
	{
		m_pAudioEngine->playBackgroundMusic(musicPath.c_str(), isLoop);
	}
}

unsigned int GameSoundEff::playSoundEff(int index, bool isLoop)
{
	CCLog("play sound music");
	if (!GameInfoSave::instance()->getPlatMusic())
	{
		return -1;
	}
	if (m_pAudioEngine && m_bCanPlayEffect)
	{
		unsigned int effecIndex = m_pAudioEngine->playEffect(m_soundEffects[index].c_str(), isLoop);
		return effecIndex;
	}
	return -1;
}

unsigned int GameSoundEff::playSoundEff(int index)
{
	return playSoundEff(index, false);
}

void GameSoundEff::parseBgMusic()
{
	if (m_pAudioEngine)
	{
		m_pAudioEngine->pauseBackgroundMusic();
	}
}

void GameSoundEff::resumeBgMusic()
{
	if (!GameInfoSave::instance()->getPlatMusic())
	{
		return;
	}

	if (m_pAudioEngine)
	{
		m_pAudioEngine->resumeBackgroundMusic();
	}
}

void GameSoundEff::stopBgMusic(bool isRelease)
{
	if (m_pAudioEngine)
	{
		m_pAudioEngine->stopBackgroundMusic(isRelease);
	}
}

void GameSoundEff::stopSoundEff(int index)
{
	if (m_pAudioEngine)
	{
		m_pAudioEngine->stopEffect(index);
	}
}

void GameSoundEff::stopAllSoundEff()
{
	if (m_pAudioEngine)
	{
		m_pAudioEngine->stopAllEffects();
	}
}

bool GameSoundEff::isPlayingBgMusic()
{
	if (m_pAudioEngine)
	{
		return m_pAudioEngine->isBackgroundMusicPlaying();
	}
	return false;
}

float GameSoundEff::getBackgroundMusicVolume()
{
	if (m_pAudioEngine)
	{
		return m_pAudioEngine->getBackgroundMusicVolume();
	}

	return 0.0f;
}

void GameSoundEff::setBackgroundMusicVolume(float volume)
{
	if (m_pAudioEngine)
	{
		m_pAudioEngine->setBackgroundMusicVolume(volume);
	}
}

float GameSoundEff::getEffectsVolume()
{
	if (m_pAudioEngine)
	{
		return m_pAudioEngine->getEffectsVolume();
	}

	return 0.0f;
}

void GameSoundEff::setEffectsVolume(float volume)
{
	if (m_pAudioEngine)
	{
		m_pAudioEngine->setEffectsVolume(volume);
	}
}

void GameSoundEff::loadSound(string file)  //加载音效+音乐
{
	if (m_pAudioEngine)
	{
		m_pAudioEngine->preloadEffect(file.c_str());
	}
}

void GameSoundEff::setPlayBgMusic(bool canPlay)
{
	m_bCanPlayBgMusic = canPlay;
}

void GameSoundEff::setPlaySound(bool canPlay)
{
	m_bCanPlayEffect = canPlay;
}