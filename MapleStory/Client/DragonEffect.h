#pragma once
#include "obj.h"

class CDragonEffect :
	public CObj
{
public:
	CDragonEffect(void);
	virtual ~CDragonEffect(void);

public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	Release(void);
	virtual void	FrameMove();

public:
	void UpdateCollRect();
};
