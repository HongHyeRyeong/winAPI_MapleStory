#pragma once
#include "obj.h"

class CGold :
	public CObj
{
public:
	CGold(void);
	~CGold(void);

public:
	void	Initialize(void);
	int		Update(void);
	void	Render(HDC hDc);
	void	Release(void);
	void	FrameMove();
	void	Jump();
	void	UpdateCollRect();

	void SetGold(int iGold);
	void SetIsXReverse(bool bIsXReverse) { m_bIsXReverse = bIsXReverse; }
	void SetOriginalY(float fOriginalY) { m_fOriginalY = fOriginalY; }

private:
	int			m_iGold;
	GOLD_ID		m_eID;
	bool		m_bIsXReverse;

protected:
	float		m_fOriginalY;

	float		m_fJumpAcc;
	float		m_fJumpSpeed;

	bool		m_bIsJump;
	bool		m_bIsJumpUp;

	float		m_fXAngle;
};
