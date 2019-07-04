#pragma once
#include "skill.h"

class CWing :
	public CSkill
{
public:
	CWing(void);
	virtual ~CWing(void);
public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	Release(void);
	virtual void	FrameMove();
	virtual void	UpdateCollRect();
};
