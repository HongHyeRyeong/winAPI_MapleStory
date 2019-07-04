#include "StdAfx.h"
#include "ObjMgr.h"
#include "Player.h"
#include "Obj.h"
#include "Rope.h"
#include "FirstFloor.h"
#include "SecondFloor.h"

IMPLEMENT_SINGLETON(CObjMgr)

void CObjMgr::AddObject(CObj* pObj, OBJECT_TYPE eID)
{
	m_ObjList[eID].push_back(pObj);
}

void CObjMgr::UpdateObj()
{
	// 이터레이터 패턴을 이용하여 한 번에 관리한다.
	for (int i = 0; i < OBJ_END; ++i)
	{
		OBJITER iter_begin = m_ObjList[i].begin();
		OBJITER iter_end = m_ObjList[i].end();
		for (; iter_begin != iter_end;)
		{
			if (1 == (*iter_begin)->Update())
			{
				SafeDelete<CObj*>(*iter_begin);
				iter_begin = m_ObjList[i].erase(iter_begin);
			}
			else	++iter_begin;
		}
	}

	if(true == g_bIsSceneChange)
		return;

	// 충돌 처리
	/////////////////////////////////////////////////////////////////////////////////////////////
	// 플레이어 - 밧줄
	CollisionMgr::CollisionRect(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_ROPE], COL_PLAYER_ROPE);
	// 플레이어 - 플로어 박스
	CollisionMgr::CollisionRect(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_SECONDFLOOR], COL_PLAYER_SECONDFLOOR);
	CollisionMgr::CollisionRect(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_FIRSTFLOOR], COL_PLAYER_FIRSTFLOOR);
	CollisionMgr::CollisionRect(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_THIRDFLOOR], COL_PLAYER_THIRDFLOOR);
	CollisionMgr::CollisionRect(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_FOURTHFLOOR], COL_PLAYER_FOURTHFLOOR);
	CollisionMgr::CollisionRect(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_FIFTHFLOOR], COL_PLAYER_FIFTHFLOOR);
	// 몬스터 - 플로어 박스
	CollisionMgr::CollisionRect(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_SECONDFLOOR], COL_MONSTER_SECONDFLOOR);
	CollisionMgr::CollisionRect(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_FIRSTFLOOR], COL_MONSTER_FIRSTFLOOR);
	CollisionMgr::CollisionRect(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_THIRDFLOOR], COL_MONSTER_THIRDFLOOR);
	CollisionMgr::CollisionRect(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_FOURTHFLOOR], COL_MONSTER_FOURTHFLOOR);
	CollisionMgr::CollisionRect(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_FIFTHFLOOR], COL_MONSTER_FIFTHFLOOR);

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////// 플레이어 - 몬스터.
	CollisionMgr::CollisionRect(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_MONSTER], COL_PLAYER_MONSTER);

	/////////////// 플레이어 - 아이템
	CollisionMgr::CollisionRect(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_ITEM], COL_PLAYER_ITEM);
	
	/////////////// 몬스터 - 스킬.
	CollisionMgr::CollisionRect(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_ARROW], COL_MONSTER_ARROW);
	CollisionMgr::CollisionRect(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_SKILL_SWING], COL_MONSTER_SWING );
	CollisionMgr::CollisionRect(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_EFFECT], COL_MONSTER_EFFECT);
	CollisionMgr::CollisionRect(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_SKILL_FIRE], COL_MONSTER_EFFECT);
	CollisionMgr::CollisionRect(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_SKILL_WING], COL_MONSTER_EFFECT);
}

void CObjMgr::RenderObj(HDC hDC)
{
	for (int i = 0; i < OBJ_END; ++i)
	{
		OBJITER iter_begin = m_ObjList[i].begin();
		OBJITER iter_end = m_ObjList[i].end();

		for (; iter_begin != iter_end; ++iter_begin)
			(*iter_begin)->Render(hDC);
	}
}

void CObjMgr::ReleaseObj(OBJECT_TYPE eID)
{
	for_each(m_ObjList[eID].begin(), m_ObjList[eID].end(), SafeDelete<CObj*>);
	m_ObjList[eID].clear();
}

void CObjMgr::ReleaseAll()
{
	for (int i = 0; i < OBJ_END; ++i)
	{
		if (i == OBJ_PLAYER || i == OBJ_HPBAR)
			continue;

		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), SafeDelete<CObj*>);
		m_ObjList[i].clear();
	}
}

 CObj* CObjMgr::GetTarget( CObj* pObj, OBJECT_TYPE eTargetID)
{
	CObj* pTarget = NULL;

	if(m_ObjList[eTargetID].empty())  return pTarget;

	OBJITER iter_begin = m_ObjList[eTargetID].begin();
	OBJITER iter_end   = m_ObjList[eTargetID].end();

	pTarget = *iter_begin;
	float fDist = CMathMgr::CalcDistance(pObj, pTarget);

	for(; iter_begin != iter_end; ++iter_begin)
	{
		float fCmpDist = CMathMgr::CalcDistance(pObj, *iter_begin);
		if(fCmpDist < fDist)
		{
			fDist = fCmpDist;
			pTarget = *iter_begin;
		}
	}
	return pTarget;
}

 OBJLIST CObjMgr::GetMonsterOBJList()
 {
	 return m_ObjList[OBJ_MONSTER];
 }

bool CObjMgr::CompareY(CObj* pDest, CObj* pSour)
{
	return pDest->GetInfo().size.cy < pSour->GetInfo().size.cy;
}

CObjMgr::CObjMgr(void)
{
}

CObjMgr::~CObjMgr(void)
{
	ReleaseAll();
}
