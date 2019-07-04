#pragma once
#include "FloorBox.h"

class CThirdFloor:
	public CFloorBox
{
public:
	CThirdFloor(void);
	virtual ~CThirdFloor(void);

public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	Release(void);
	virtual void	FrameMove();

};
