#pragma once
#include "NPC.h"

class CLeaf :
	public CNPC
{
public:
	CLeaf(void);
	virtual ~CLeaf(void);

public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	Release(void);
	virtual void	FrameMove();

private:
	template <typename T>
	CObj*	CreateCommunication()
	{
		CObj* pLeafCommunication = CAbstractFactory<T>::CreateCommunication(
			WINCX * 0.5f, WINCY * 0.5f, m_iCommunicationNumber, m_bIsQuestSucessed);
		return pLeafCommunication;
	}

private:
	RECT m_tMouseToRect;
};
