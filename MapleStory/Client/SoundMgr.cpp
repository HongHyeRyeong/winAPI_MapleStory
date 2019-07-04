#include "StdAfx.h"
#include "SoundMgr.h"

IMPLEMENT_SINGLETON(CSoundMgr)

CSoundMgr::CSoundMgr(void)
: m_pSystem(NULL)
{
	Initialize();
}

CSoundMgr::~CSoundMgr(void)
{
	Release();
}

void CSoundMgr::Initialize()
{
	FMOD_System_Create(&m_pSystem);
	FMOD_System_Init(m_pSystem, CHANNEL_END, FMOD_INIT_NORMAL, NULL);

	LoadSoundFile();
}

void CSoundMgr::LoadSoundFile()
{
	_finddata_t fd;

	long handle = _findfirst("../Sound/*.*", &fd);

	if( -1 == handle )
	{
		MessageBox(g_hWnd, L"Not found Sound file", L"Not found Sound file", MB_OK);
		return;
	}

	int iResult = 0;

	while(-1 != iResult)
	{
		TCHAR* pSoundKey = new TCHAR[256];
		ZeroMemory(pSoundKey, sizeof(TCHAR) * 256);

		MultiByteToWideChar(CP_ACP, 0, fd.name, strlen(fd.name) + 1, pSoundKey, 256);

		char szFullpath[256] = "";
		strcpy_s(szFullpath, 256, "../Sound/");
		strcat_s(szFullpath, 256, fd.name);

		FMOD_SOUND*	pSound = NULL;

		if(FMOD_OK == FMOD_System_CreateSound(m_pSystem, szFullpath, FMOD_HARDWARE, NULL, &pSound))
		{
			m_MapSound.insert(pair<TCHAR*, FMOD_SOUND*>(pSoundKey, pSound));
		}
		else
			delete[] pSoundKey;

		iResult = _findnext(handle, &fd);
	}

	_findclose(handle);

	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::PlaySound(const TCHAR* pSoundKey, CHANNEL_ID eChannel)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter_find = find_if(m_MapSound.begin(), m_MapSound.end()
		, CStringCmp(pSoundKey));

	if(m_MapSound.end() == iter_find)
		return;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter_find->second, 0, &m_pChannelArr[eChannel]);
}

void CSoundMgr::PlayBGM(const TCHAR* pSoundKey)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter_find = find_if(m_MapSound.begin(), m_MapSound.end()
		, CStringCmp(pSoundKey));

	if(m_MapSound.end() == iter_find)
		return;
	
	FMOD_Sound_SetMode(iter_find->second, FMOD_LOOP_NORMAL);
	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter_find->second, 0, &m_pChannelArr[CHANNEL_BGM]);
}

void CSoundMgr::UpdateSound()
{
	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::StopSound(CHANNEL_ID eChannel)
{
	FMOD_Channel_Stop(m_pChannelArr[eChannel]);
}

void CSoundMgr::StopSoundAll()
{
	for(int i = 0; i < CHANNEL_END; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void CSoundMgr::Release()
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter_begin = m_MapSound.begin();
	map<TCHAR*, FMOD_SOUND*>::iterator iter_end = m_MapSound.end();

	for( ; iter_begin != iter_end; ++iter_begin)
	{
		delete[] iter_begin->first;
		FMOD_Sound_Release(iter_begin->second);
	}

	m_MapSound.clear();

	FMOD_System_Close(m_pSystem);
	FMOD_System_Release(m_pSystem);
}