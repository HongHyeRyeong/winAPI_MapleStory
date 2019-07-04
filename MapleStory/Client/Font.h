#pragma once
#include "obj.h"

class CFont :
	public CObj
{
public:
	CFont(void);
	virtual ~CFont(void);

public:
	virtual void	Initialize(void);
	virtual int		Update(void);
	virtual void	Render(HDC hDc);
	virtual void	Release(void);
	virtual void	FrameMove();

private:
	DWORD		m_dwLifeCurTime; // 생성된 후 없어지는 시간
	DWORD		m_dwLifeOldTime; // 생성된 후 없어지는 시간

public:
	void SetObj(CObj* pObj) { m_pObj = pObj; }
	void SetNumber(int iNumber) { m_iNumber = iNumber; }
	void SetType(FONT_TYPE eType) { m_eType = eType; }
	void SetID(FONT_ID eID) { m_eID = eID; }
	void SetFloor(FONT_FLOOR eFloor) { m_eFloor = eFloor; }

public:
	void SettingPos();
	void Calculating();
	int ReturningRandomNumber(int iNumber);

private:
	typedef struct tagPos
	{
		MYPOINT pt;
	}POS;

private:
	int			m_iNumber;

private:
	CObj * m_pObj;
	FONT_TYPE	m_eType;
	FONT_ID		m_eID;
	FONT_FLOOR	m_eFloor;

	POS			m_tPosArr[5];

private:
	RECT		m_tUnitRect;
	RECT		m_tTensRect;
	RECT		m_tHundRect;
	RECT		m_tThouRect;
	RECT		m_tTenthRect;

	INFO		m_tUnitInfo;
	INFO		m_tTensInfo;
	INFO		m_tHundInfo;
	INFO		m_tThouInfo;
	INFO		m_tTenthInfo;

	FRAME		m_tUnitFrame;
	FRAME		m_tTensFrame;
	FRAME		m_tHundFrame;
	FRAME		m_tThouFrame;
	FRAME		m_tTenthFrame;
};

