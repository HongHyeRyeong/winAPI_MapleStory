#pragma once
#include "skill.h"

class CDragon :
	public CSkill
{
public:
	CDragon(void);
	virtual ~CDragon(void);

public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	Release(void);
	virtual void	FrameMove();
	virtual void	UpdateCollRect();
};
