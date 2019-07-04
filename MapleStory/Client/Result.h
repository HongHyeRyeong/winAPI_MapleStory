#pragma once
#include "obj.h"

class CResult :
	public CObj
{
public:
	CResult();
	virtual ~CResult();

public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	Release(void) {};
	virtual void	FrameMove() {};

	void SetShowed(bool bShow);
private:
	bool	m_bIsShowed; // on/off ±â´É

	RECT	m_tExitButton;
};

