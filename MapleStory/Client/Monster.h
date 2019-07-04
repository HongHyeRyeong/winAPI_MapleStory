#pragma once
#include "obj.h"
#include "Gold.h"

class CMonster :
	public CObj
{
public:
	CMonster(void);
	virtual ~CMonster(void);

public:
	virtual void	UpdateCollRect() = 0;

	void SetMoney(int iMoney) { m_tState.iGold = iMoney; }

	void SetIsFloorBoxColl(bool bFloorBoxColl) { m_bIsFloorBoxColl = bFloorBoxColl; }
	bool GetIsFloorBoxColl() { return m_bIsFloorBoxColl; }

public:
	MONSTER_STATE GetMonsterState() { return m_eCurState; }
	void		  SetMonsterState(MONSTER_STATE eState) { m_eCurState = eState; }

public:
	template <typename T>
	CObj*	CreateGold()
	{
		float fHeight =  static_cast<float>(m_tCollRect.bottom - 17.5 * 0.5f);
		float fWidth = 15.f;
		CObj* pGold = CAbstractFactory<T>::CreateObj(
			m_tInfo.pt.x - fWidth, fHeight);
		dynamic_cast<CGold*>(pGold)->SetGold(m_tState.iGold);
		dynamic_cast<CGold*>(pGold)->SetOriginalY(m_tInfo.pt.y);
		return pGold;
	}

protected:
	MONSTER_STATE   m_eCurState;
	MONSTER_STATE	m_ePreState;

	bool		m_bCombatible;

	DWORD		m_dwCreateCurTime;
	DWORD		m_dwCreateOldTime;

	bool		m_bIsFloorBoxColl;
};
