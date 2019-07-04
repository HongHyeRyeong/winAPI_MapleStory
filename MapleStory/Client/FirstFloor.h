#pragma once
#include "FloorBox.h"

class CFirstFloor :
	public CFloorBox
{
public:
	CFirstFloor(void);
	virtual ~CFirstFloor(void);

public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	Release(void);
	virtual void	FrameMove();


};
