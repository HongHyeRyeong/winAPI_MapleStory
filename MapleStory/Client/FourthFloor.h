#pragma once
#include "FloorBox.h"

class CFourthFloor :
	public CFloorBox
{
public:
	CFourthFloor(void);
	virtual ~CFourthFloor(void);
public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	Release(void);
	virtual void	FrameMove();

};
