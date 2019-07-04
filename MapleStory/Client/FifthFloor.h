#pragma once
#include "FloorBox.h"


class CFifthFloor:
	public CFloorBox
{
public:
	CFifthFloor(void);
	virtual ~CFifthFloor(void);

public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	Release(void);
	virtual void	FrameMove();

};
