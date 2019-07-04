#pragma once
#include "Obj.h"
#include "Monster.h"

class CGreen :
	public CMonster
{
public:
	CGreen(void);
	virtual ~CGreen(void);

public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	Release(void);
	virtual void	FrameMove();
	virtual void	UpdateCollRect();

	void	LineCollision();
};
