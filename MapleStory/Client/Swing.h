#pragma once
#include "Skill.h"

class CSwing :
	public CSkill
{
public:
	CSwing(void);
	virtual ~CSwing(void);

public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	Release(void);
	virtual void	FrameMove();
	virtual void	UpdateCollRect();
};
