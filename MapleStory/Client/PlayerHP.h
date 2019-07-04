#pragma once
#include "obj.h"

class CPlayerHP :
	public CObj
{
public:
	CPlayerHP(void);
	virtual ~CPlayerHP(void);

public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	FrameMove();
	virtual void	Release(void);

public:
	void SetPlayer(CObj* pPlayer) { m_pPlayer = pPlayer; }

private:
	CObj*	m_pPlayer;
	int		m_iHp;
	int		m_iHpMax;
};
