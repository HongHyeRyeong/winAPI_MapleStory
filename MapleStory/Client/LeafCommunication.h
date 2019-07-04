#pragma once
#include "Commuication.h"

class CLeafCommunication :
	public CCommuication
{
public:
	CLeafCommunication(void);
	virtual ~CLeafCommunication(void);

public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	Release(void);
	virtual void	FrameMove();

private:
	bool m_bIsNextButtonActivated;
};
