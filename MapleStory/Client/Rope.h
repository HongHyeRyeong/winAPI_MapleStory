#pragma once
#include "obj.h"

class CRope :
	public CObj
{
public:
	CRope(void);
	virtual ~CRope(void);

public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	Release(void);
	virtual void	FrameMove();
};
