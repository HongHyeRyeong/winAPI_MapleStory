#include "StdAfx.h"
#include "BitmapMgr.h"

IMPLEMENT_SINGLETON(CBitmapMgr)

CBitmapMgr::CBitmapMgr(void)
{
}

CBitmapMgr::~CBitmapMgr(void)
{
	Release();
}

CMyBmp* CBitmapMgr::FindImage(const TCHAR* pFindKey)
{
	map<const TCHAR*, CMyBmp*>::iterator iter_find
		= find_if(m_MapBit.begin(), m_MapBit.end(), CStringCmp(pFindKey));

	if(iter_find == m_MapBit.end())
	{
		MessageBox(g_hWnd, L"Image Not Found!!", L"Image Not Found!!", MB_OK);
		return NULL;
	}

	return iter_find->second;
}

void CBitmapMgr::LoadImageByScene(SCENE_TYPE eType)	// 모든 씬에 들어갈 이미지
{
	m_MapBit.insert(make_pair(
		L"Mouse", (new CMyBmp)->LoadBmp(L"../Image/Object/UI/Mouse.bmp")));

	// 플레이어
	m_MapBit.insert(make_pair(
		L"Player_LEFT", (new CMyBmp)->LoadBmp(L"../Image/Object/Player/Player_LEFT.bmp")));
	m_MapBit.insert(make_pair(
		L"Player_RIGHT", (new CMyBmp)->LoadBmp(L"../Image/Object/Player/Player_RIGHT.bmp")));
	m_MapBit.insert(make_pair(
		L"Player_ROPE", (new CMyBmp)->LoadBmp(L"../Image/Object/Player/Player_ROPE.bmp")));
	m_MapBit.insert(make_pair(
		L"Captin_LEFT", (new CMyBmp)->LoadBmp(L"../Image/Object/Player/Captin_LEFT.bmp")));
	m_MapBit.insert(make_pair(
		L"Captin_RIGHT", (new CMyBmp)->LoadBmp(L"../Image/Object/Player/Captin_RIGHT.bmp")));
	m_MapBit.insert(make_pair(
		L"Captin_ROPE", (new CMyBmp)->LoadBmp(L"../Image/Object/Player/Captin_ROPE.bmp")));

	m_MapBit.insert(make_pair(
		L"Shoot_LEFT", (new CMyBmp)->LoadBmp(L"../Image/Object/Player/Attack/Shoot/Shoot_LEFT.bmp")));
	m_MapBit.insert(make_pair(
		L"Shoot_RIGHT", (new CMyBmp)->LoadBmp(L"../Image/Object/Player/Attack/Shoot/Shoot_RIGHT.bmp")));
	m_MapBit.insert(make_pair(
		L"Swing_RIGHT", (new CMyBmp)->LoadBmp(L"../Image/Object/Player/Attack/Swing/Swing_RIGHT.bmp")));
	m_MapBit.insert(make_pair(
		L"Swing_LEFT", (new CMyBmp)->LoadBmp(L"../Image/Object/Player/Attack/Swing/Swing_LEFT.bmp")));
	m_MapBit.insert(make_pair(
		L"Arrow_LEFT", (new CMyBmp)->LoadBmp(L"../Image/Object/Player/Attack/Shoot/Arrow_LEFT.bmp")));
	m_MapBit.insert(make_pair(
		L"Arrow_RIGHT", (new CMyBmp)->LoadBmp(L"../Image/Object/Player/Attack/Shoot/Arrow_RIGHT.bmp")));
	m_MapBit.insert(make_pair(
		L"SwingEffect_RIGHT", (new CMyBmp)->LoadBmp(L"../Image/Object/Player/Attack/Swing/SwingEffect_RIGHT.bmp")));
	m_MapBit.insert(make_pair(
		L"SwingEffect_LEFT", (new CMyBmp)->LoadBmp(L"../Image/Object/Player/Attack/Swing/SwingEffect_LEFT.bmp")));
	m_MapBit.insert(make_pair(
		L"Dragon_LEFT", (new CMyBmp)->LoadBmp(L"../Image/Object/Player/Skill/Dragon/Dragon_LEFT.bmp")));
	m_MapBit.insert(make_pair(
		L"Dragon_RIGHT", (new CMyBmp)->LoadBmp(L"../Image/Object/Player/Skill/Dragon/Dragon_RIGHT.bmp")));
	m_MapBit.insert(make_pair(
		L"Fire", (new CMyBmp)->LoadBmp(L"../Image/Object/Player/Skill/Fire/FireEffect.bmp")));
	m_MapBit.insert(make_pair(
		L"Wing", (new CMyBmp)->LoadBmp(L"../Image/Object/Player/Skill/Wing/WingEffect.bmp")));
	m_MapBit.insert(make_pair(
		L"DragonEffect", (new CMyBmp)->LoadBmp(L"../Image/Object/Player/Skill/Dragon/DragonEffect.bmp")));

	// 이펙트
	m_MapBit.insert(make_pair(
		L"LevelUp", (new CMyBmp)->LoadBmp(L"../Image/Object/LevelUp.bmp")));

	// 폰트
	m_MapBit.insert(make_pair(
		L"Font", (new CMyBmp)->LoadBmp(L"../Image/Object/Font.bmp")));
	m_MapBit.insert(make_pair(
		L"LvNum", (new CMyBmp)->LoadBmp(L"../Image/Object/LvNum.bmp")));

	// UI
	m_MapBit.insert(make_pair(
		L"UI", (new CMyBmp)->LoadBmp(L"../Image/Object/UI/MainUI.bmp")));
	m_MapBit.insert(make_pair(
		L"PlayerHPBar", (new CMyBmp)->LoadBmp(L"../Image/Object/UI/PlayerHPBar.bmp")));
	m_MapBit.insert(make_pair(
		L"PlayerMPBar", (new CMyBmp)->LoadBmp(L"../Image/Object/UI/PlayerMPBar.bmp")));
	m_MapBit.insert(make_pair(
		L"PlayerEXPBar", (new CMyBmp)->LoadBmp(L"../Image/Object/UI/PlayerEXPBar.bmp")));

	switch (eType)
	{
	case SCENE_LOGO:
		m_MapBit.insert(make_pair
		(L"Logo1", (new CMyBmp)->LoadBmp(L"../Image/Logo/Logo1.bmp")));
		m_MapBit.insert(make_pair
		(L"Logo2", (new CMyBmp)->LoadBmp(L"../Image/Logo/Logo2.bmp")));
		m_MapBit.insert(make_pair
		(L"Logo3", (new CMyBmp)->LoadBmp(L"../Image/Logo/Logo3.bmp")));
		m_MapBit.insert(make_pair
		(L"Login", (new CMyBmp)->LoadBmp(L"../Image/Logo/Login.bmp")));
		m_MapBit.insert(make_pair
		(L"Exit", (new CMyBmp)->LoadBmp(L"../Image/Logo/Exit.bmp")));
		break;
	case SCENE_MAKINGPLAYER:
		m_MapBit.insert(make_pair
		(L"MakingPlayer1", (new CMyBmp)->LoadBmp(L"../Image/MakingPlayer/MakingPlayer1.bmp")));
		m_MapBit.insert(make_pair
		(L"MakingPlayer2", (new CMyBmp)->LoadBmp(L"../Image/MakingPlayer/MakingPlayer2.bmp")));
		break;
	case SCENE_FIELD:
		m_MapBit.insert(make_pair
		(L"BackGround", (new CMyBmp)->LoadBmp(L"../Image/Map/Field.bmp")));
		m_MapBit.insert(make_pair
		(L"Leaf", (new CMyBmp)->LoadBmp(L"../Image/Object/Leaf.bmp")));
		m_MapBit.insert(make_pair
		(L"QuestMark", (new CMyBmp)->LoadBmp(L"../Image/Object/UI/QuestMark.bmp")));
		m_MapBit.insert(make_pair
		(L"Communication_Leaf", (new CMyBmp)->LoadBmp(L"../Image/Object/UI/Communication_Leaf.bmp")));
		m_MapBit.insert(make_pair
		(L"Party", (new CMyBmp)->LoadBmp(L"../Image/Object/UI/Party.bmp")));
		break;
	case SCENE_STAGE1:
		m_MapBit.insert(make_pair
		(L"BackGround_Henesis", (new CMyBmp)->LoadBmp(L"../Image/Map/Henesis.bmp")));
		m_MapBit.insert(make_pair
		(L"GreenMush_LEFT", (new CMyBmp)->LoadBmp(L"../Image/Object/Monster/GreenMush/GreenMush_LEFT.bmp")));
		m_MapBit.insert(make_pair
		(L"GreenMush_RIGHT", (new CMyBmp)->LoadBmp(L"../Image/Object/Monster/GreenMush/GreenMush_RIGHT.bmp")));
		m_MapBit.insert(make_pair
		(L"CoupleMush_LEFT", (new CMyBmp)->LoadBmp(L"../Image/Object/Monster/CoupleMush/CoupleMush_LEFT.bmp")));
		m_MapBit.insert(make_pair
		(L"CoupleMush_RIGHT", (new CMyBmp)->LoadBmp(L"../Image/Object/Monster/CoupleMush/CoupleMush_RIGHT.bmp")));
		m_MapBit.insert(make_pair
		(L"Gold", (new CMyBmp)->LoadBmp(L"../Image/Object/Gold.bmp")));
		m_MapBit.insert(make_pair
		(L"Result", (new CMyBmp)->LoadBmp(L"../Image/Object/UI/Result.bmp")));
		break;
	}
}

void CBitmapMgr::Release()
{

	map<const TCHAR*, CMyBmp*>::iterator iter_begin = m_MapBit.begin();
	map<const TCHAR*, CMyBmp*>::iterator iter_end	= m_MapBit.end();

	for(; iter_begin != iter_end; ++iter_begin)
	{
		delete iter_begin->second;
		iter_begin->second = NULL;
	}
	m_MapBit.clear();
}
