#pragma once
#include "skill.h"

class CFire :
	public CSkill
{
public:
	CFire(void);
	virtual ~CFire(void);

public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	Release(void);
	virtual void	FrameMove();
	virtual void	UpdateCollRect();
};
