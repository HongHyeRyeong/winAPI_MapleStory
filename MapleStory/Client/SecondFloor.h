#pragma once
#include "FloorBox.h"

class CSecondFloor :
	public CFloorBox
{
public:
	CSecondFloor(void);
	virtual ~CSecondFloor(void);

public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	Release(void);
	virtual void	FrameMove();


};
