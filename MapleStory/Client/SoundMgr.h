#pragma once

class CSoundMgr
{
	DECLARE_SINGLETON(CSoundMgr)
public:
	enum CHANNEL_ID { CHANNEL_PLAYER, CHANNEL_MONSTER,  CHANNEL_SKILL, CHANNEL_EFFECT, CHANNEL_BGM, CHANNEL_END };

private:
	CSoundMgr(void);
	~CSoundMgr(void);

public:
	void Initialize();
	void LoadSoundFile();
	void PlaySound(const TCHAR* pSoundKey, CHANNEL_ID eChannel);
	void PlayBGM(const TCHAR* pSoundKey);
	void UpdateSound();
	void StopSound(CHANNEL_ID eChannel);
	void StopSoundAll();
	void Release();

private:
	FMOD_SYSTEM*	m_pSystem; // 사운드를 관리할 시스템 포인터.
	FMOD_CHANNEL*	m_pChannelArr[CHANNEL_END];	// 사운드를 재생할 채널의 집합.

	map<TCHAR*, FMOD_SOUND*> m_MapSound;
};
