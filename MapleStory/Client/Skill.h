#pragma once
#include "obj.h"

class CSkill :
	public CObj
{
public:
	CSkill(void);
	virtual ~CSkill(void);

public:
	virtual void	UpdateCollRect() = 0;
	void SetPlayer(CObj* pPlayer) { m_pPlayer = pPlayer; }

protected:
	CObj * m_pPlayer;
};
