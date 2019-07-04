#pragma once
#include "obj.h"

class CPlayerMP :
	public CObj
{
public:
	CPlayerMP(void);
	virtual ~CPlayerMP(void);

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
	int		m_iMp;
	int		m_iMpMax;
};
