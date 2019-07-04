#pragma once
#include "obj.h"

class CPlayerExp :
	public CObj
{
public:
	CPlayerExp(void);
	virtual ~CPlayerExp(void);

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
	int		m_iExp;
	int		m_iExpMax;
};
