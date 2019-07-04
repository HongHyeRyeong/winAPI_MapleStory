#pragma once
#include "obj.h"

class CCommuication :
	public CObj
{
public:
	CCommuication(void);
	virtual ~CCommuication(void);

public:
	void SetRenderNumber(int iNumber) { m_tFrame.iFrameStart = iNumber; }
	virtual void SetNextButtonRect();

protected:
	int		m_iRenderNumber;
	int		m_iRenderNumberOld;

	RECT	m_tNextButton;
};
