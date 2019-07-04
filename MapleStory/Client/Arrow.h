#pragma once
#include "Skill.h"

class CArrow :
	public CSkill
{
public:
	CArrow(void);
	virtual ~CArrow(void);

public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	Release(void);
	virtual void	FrameMove();
	virtual void	UpdateCollRect();

public:
	void SetID(ARROW_ID eID) { m_eID = eID; }
	void SetOrder(int iOrder) { m_iOrder = iOrder; }
	int GetOrder() { return m_iOrder; }

	void SetCollMode(bool bCollMode) { m_bCollMode = bCollMode; }
	bool GetCollMode() { return m_bCollMode; }

private:
	template <typename T>
	CObj*	CreateEffect()
	{
		CObj* pEffect = CAbstractFactory<T>::CreateObj(
			m_tInfo.pt.x, m_tInfo.pt.y);
		//pEffect->SetAtt(this->GetState().iAtt);
		//pEffect->SetDir(m_eDir);
		return pEffect;
	}

private:
	ARROW_ID	m_eID;
	int			m_iOrder;

	bool		m_bCollMode;

};
