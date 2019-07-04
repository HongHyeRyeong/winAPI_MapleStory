#include "StdAfx.h"
#include "SceneMgr.h"
#include "Logo.h"
#include "MakingPlayer.h"
#include "Field.h"
#include "Stage1.h"

IMPLEMENT_SINGLETON(CSceneMgr)

CSceneMgr::CSceneMgr(void)
: m_pScene(NULL), m_bChangeCheck(false)
{
}

CSceneMgr::~CSceneMgr(void)
{
	Release();
}

void CSceneMgr::SetScene(SCENE_TYPE eType)
{
	m_eSceneType = eType;

	// Field에 있던 Player 포인터를, Stage1가 갖고 있도록 넘겨주어야 한다.
	CPlayer* pPlayer = nullptr;
	CPlayer* pOtherPlayer = nullptr;
	// 기존에 Scene이 할당 되어 있다면 해제할 것
	if (NULL != m_pScene)
	{
		if (g_eScene == SCENE_STAGE1) {
			// 스테이지1로 넘어가고 필드 씬을 지우기 전에! player 포인터를 넘겨 준다.
			pPlayer = dynamic_cast<CField*>(m_pScene)->GetPlayer();
			pOtherPlayer = dynamic_cast<CField*>(m_pScene)->GetOtherPlayer();
		}
		SafeDelete<CScene*>(m_pScene);
		//g_fScrollX = 0.f;
		//g_fScrollY = 0.f;
	}

	switch (eType)
	{
	case SCENE_LOGO:
		m_pScene = new CLogo;
		break;
	case SCENE_MAKINGPLAYER:
		m_pScene = new CMakingPlayer;
		break;
	case SCENE_FIELD:
		m_pScene = new CField;
		break;
	case SCENE_STAGE1:
		m_pScene = new CStage1;
		if (pPlayer != nullptr && pOtherPlayer != nullptr) {
			dynamic_cast<CStage1*>(m_pScene)->SetPlayer(pPlayer);
			dynamic_cast<CStage1*>(m_pScene)->SetPlayer(pOtherPlayer);
		}
		break;
	}
	m_pScene->Initialize();
}

void CSceneMgr::Update()
{
	if(m_bChangeCheck)
	{
		m_bChangeCheck = false;
		SetScene(m_eSceneType);
	}

	if(m_pScene)
		m_pScene->Update();
}

void CSceneMgr::Render(HDC hDc)
{
	if(m_pScene)
		m_pScene->Render(hDc);
}

void CSceneMgr::Release()
{
	SafeDelete<CScene*>(m_pScene);
}

void CSceneMgr::SetChangeScene(SCENE_TYPE eType)
{
	m_eSceneType = eType;
	m_bChangeCheck = true;
}
