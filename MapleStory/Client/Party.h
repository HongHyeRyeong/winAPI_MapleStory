#pragma once
#include "obj.h"

class CParty :
	public CObj
{
public:
	CParty();
	virtual ~CParty();

public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	Release(void) {};
	virtual void	FrameMove() {};

private:
	bool			m_bIsShowed; // on/off ±â´É
};

