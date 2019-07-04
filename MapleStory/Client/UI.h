#pragma once
#include "obj.h"

class CUI :
	public CObj
{
public:
	CUI(void);
	virtual ~CUI(void);

public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	FrameMove();
	virtual void	Release(void);

private:
	int		m_iTensLevel;	// 십의 자리
	RECT	m_tTensLvRect;
	INFO	m_tTensLvInfo;
	FRAME   m_tTensFrame;

	int		m_iUnitLevel;	// 일의 자리
	RECT	m_tUnitLvRect;
	INFO	m_tUnitLvInfo;
	FRAME   m_tUnitFrame;
};
