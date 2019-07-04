#pragma once
#include "obj.h"

class CNPC :
	public CObj
{
public:
	CNPC(void);
	virtual ~CNPC(void);

protected:
	bool m_bIsActivated;
	bool m_bIsQuestSucessed;
	int  m_iCommunicationNumber;
};
