#pragma once
#include "obj.h"

class CFloorBox :
	public CObj
{
public:
	CFloorBox(void);
	virtual ~CFloorBox(void);

public:
	FLOORBOX_ID GetFloorBoxID(void) { return m_eID; }
	void		SetFloorBoxID(FLOORBOX_ID eID) { m_eID = eID; }

protected:
	FLOORBOX_ID		m_eID;
};
