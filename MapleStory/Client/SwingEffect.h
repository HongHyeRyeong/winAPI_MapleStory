#pragma once
#include "obj.h"

class CSwingEffect :
	public CObj
{
public:
	CSwingEffect(void);
	virtual ~CSwingEffect(void);

public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	Release(void);
	virtual void	FrameMove();

public:
	void UpdateCollRect();
};
